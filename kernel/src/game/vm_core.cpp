//
//
#include <sge/vm/kernel.hpp>

SGE_VM_BEGIN

kernel::kernel(uv_loop_t *loop):
    core(loop)
{
}

kernel::~kernel(void)
{
}

bool kernel::start(const std::string &rootfs, const std::string &initrc)
{
    uv_async_init(loop(), &m_trap_async, &kernel::handle_traps);
    m_trap_async.data = this;

    if (!core::start(rootfs, initrc)) {
        uv_sync_close(reinterpret_cast<uv_handle_t *>(&m_trap_async));
        return false;
    }

    return true;
}

void kernel::stop(void)
{
    core::stop();
    uv_sync_close(reinterpret_cast<uv_handle_t *>(&m_trap_async));
}

void kernel::task_added(lua_State *L, lua_State *L1)
{
    task_t *main = task_from_lua(L);
    task_t *task = task_from_lua(L1);

    uv_timer_init(main->loop, &task->sleep_timer);

    task->loop = main->loop;
    task->kernel = main->kernel;
    task->sleep_timer.data = task;
    task->trap_func = nullptr;
    task->trap_result = TRAP_ERROR_NOT_SET;

    sge_list_reset(&task->wait_list);
    sge_list_node_reset(&task->node);

    std::unique_lock locker(from_task(task)->m_task_list_lock);
    sge_list_add_tail(&from_task(task)->m_task_list_ready, &task->node);
}

void kernel::task_removed(lua_State *L, lua_State *L1)
{
    SGE_UNUSED(L);

    task_t *task = task_from_lua(L1);

    uv_timer_stop(&task->sleep_timer);
    uv_sync_close(&task->sleep_timer);

    std::unique_lock locker(from_task(task)->m_task_list_lock);
    sge_list_node_unlink(&task->node);
}

void kernel::task_resume(lua_State *L, int n)
{
    SGE_UNUSED(L);
    SGE_UNUSED(n);
}

void kernel::task_yield(lua_State *L, int n)
{
    SGE_UNUSED(L);
    SGE_UNUSED(n);
}

void kernel::handle_event(const SDL_Event &evt)
{
    core::handle_event(evt);
}

void kernel::run(uv_loop_t *loop)
{
    sge_list_reset(&m_task_list_ready);
    sge_list_reset(&m_task_list_trapped);

    lua_State *L = luaL_newstate();
    if (L == nullptr) {
        set_init_result(false);
        return;
    }

    lua_pushcfunction(L, [](lua_State *L) -> int {
        kernel *k = reinterpret_cast<kernel *>(lua_touserdata(L, 1));
        uv_loop_t *loop = reinterpret_cast<uv_loop_t *>(lua_touserdata(L, 2));
        k->kmain(L, loop);
        return 0;
    });

    lua_pushlightuserdata(L, this);
    lua_pushlightuserdata(L, loop);
    lua_pcall(L, 2, 0, 0);
    lua_close(L);
}

void kernel::kmain(lua_State *L, uv_loop_t *loop)
{
    luaL_openlibs(L);

    init_exports(L);

    task_t *main = task_from_lua(L);
    main->kernel = this;
    main->loop = loop;

    if (!load_initrc(L)) {
        set_init_result(false);
        return;
    }

    uv_prepare_t schedule_preparer;
    uv_prepare_init(loop, &schedule_preparer);
    uv_prepare_start(&schedule_preparer, &kernel::schedule);
    schedule_preparer.data = L;

    core::run(loop);

    uv_prepare_stop(&schedule_preparer);
    uv_sync_close(&schedule_preparer);
}

bool kernel::load_initrc(lua_State *L)
{
    return true;
}

void kernel::schedule(uv_prepare_t *p)
{
    sge_list_node_t *node;
    task_t *task;
    lua_State *T;
    int ret;
    int nresults;
    lua_State *L = reinterpret_cast<lua_State *>(p->data);
    kernel *k = from_lua(L);

    for (;;) {
        k->m_task_list_lock.lock();
        node = sge_list_del_head(&k->m_task_list_ready);
        k->m_task_list_lock.unlock();
        if (node == sge_list_knot(&k->m_task_list_ready))
            break;
        task = SGE_MEMBEROF(node, task_t, node);
        T = task_to_lua(task);
        ret = lua_resume(T, L, 0, &nresults);
        if (ret != LUA_YIELD) {
            k->m_task_list_lock.lock();
            sge_list_move(&k->m_task_list_ready, &task->wait_list);
            k->m_task_list_lock.unlock();
        }
    }

    k->m_task_list_lock.lock();
    if (!sge_list_empty(&k->m_task_list_trapped)) {
        k->m_task_list_lock.unlock();
        uv_async_send(&k->m_trap_async);
    } else
        k->m_task_list_lock.unlock();
}

void kernel::handle_traps(uv_async_t *p)
{
    kernel *k = reinterpret_cast<kernel *>(p->data);
    sge_list_node_t *node;
    task_t *task;
    bool handled = false;

    for (;;) {
        k->m_task_list_lock.lock();
        node = sge_list_del_head(&k->m_task_list_trapped);
        k->m_task_list_lock.unlock();

        if (node != sge_list_knot(&k->m_task_list_trapped)) {
            task = SGE_MEMBEROF(node, task_t, node);
            task->trap_result = task->trap_func(task_to_lua(task));
            handled = true;
        } else
            break;

        k->m_task_list_lock.lock();
        sge_list_add_tail(&k->m_task_list_ready, &task->node);
        k->m_task_list_lock.unlock();
    }

    if (handled)
        k->kick();
}

int kernel::trap_done(lua_State *L, int status, lua_KContext ctx)
{
    SGE_UNUSED(status);
    SGE_UNUSED(ctx);

    task_t *task = task_from_lua(L);
    SGE_ASSERT(task->trap_func != nullptr);

    int ret = task->trap_result;
    if (ret < 0)
        luaL_error(L, "trap %p error %d", task->trap_func, ret);

    task->trap_func = nullptr;
    task->trap_result = 0;

    return ret;
}

int kernel::do_trap(lua_State *L, lua_CFunction func)
{
    SGE_ASSERT(func != nullptr);

    task_t *task = task_from_lua(L);
    SGE_ASSERT(task->trap_func == nullptr);

    kernel *k = from_task(task);
    SGE_ASSERT(std::this_thread::get_id() == k->thread().get_id());

    task->trap_func = func;
    task->trap_result = TRAP_ERROR_NOT_SET;

    k->m_task_list_lock.lock();
    sge_list_add_tail(&k->m_task_list_trapped, &task->node);
    k->m_task_list_lock.unlock();

    return lua_yieldk(L, 0, 0, &kernel::trap_done);
}

SGE_VM_END

extern "C" void sge_vm_task_added(lua_State *L, lua_State *L1)
{
    sge::vm::kernel::task_added(L, L1);
}

extern "C" void sge_vm_task_removed(lua_State *L, lua_State *L1)
{
    sge::vm::kernel::task_removed(L, L1);
}

extern "C" void sge_vm_task_resume(lua_State *L, int n)
{
    sge::vm::kernel::task_resume(L, n);
}

extern "C" void sge_vm_task_yield(lua_State *L, int n)
{
    sge::vm::kernel::task_yield(L, n);
}
