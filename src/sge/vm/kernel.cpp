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
    task->trap_type = TRAP_TYPE_INVALID;
    task->trap_result = TRAP_ERROR_NOT_SET;

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

void kernel::init_exports(lua_State *L)
{
    lua_newtable(L);

    lua_pushcfunction(L, &kernel::sys_task);
    lua_setfield(L, -2, "task");

    lua_pushcfunction(L, &kernel::sys_current);
    lua_setfield(L, -2, "current");

    lua_pushcfunction(L, &kernel::sys_wait);
    lua_setfield(L, -2, "wait");

    lua_pushcfunction(L, &kernel::sys_sleep);
    lua_setfield(L, -2, "sleep");

    lua_setglobal(L, "sge");
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
        if (node != sge_list_knot(&k->m_task_list_ready)) {
            task = SGE_MEMBEROF(node, task_t, node);
            T = task_to_lua(task);
            ret = lua_resume(T, L, 0, &nresults);
            /* TODO ret */
        } else
            break;
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
    lua_State *L;
    bool handled = false;

    for (;;) {
        k->m_task_list_lock.lock();
        node = sge_list_del_head(&k->m_task_list_trapped);
        k->m_task_list_lock.unlock();
        if (node != sge_list_knot(&k->m_task_list_trapped)) {
            task = SGE_MEMBEROF(node, task_t, node);
            L = task_to_lua(task);
            switch (task->trap_type) {
            case TRAP_TYPE_FOO:
                task->trap_result = k->trap_foo(L);
                break;
            default:
                task->trap_result = TRAP_ERROR_UNKNOWN;
                break;
            }
            k->m_task_list_lock.lock();
            sge_list_add_tail(&k->m_task_list_ready, &task->node);
            k->m_task_list_lock.unlock();
            handled = true;
        } else
            break;
    }

    if (handled)
        k->kick();
}

int kernel::trap_done(lua_State *L, int status, lua_KContext ctx)
{
    SGE_UNUSED(status);
    SGE_UNUSED(ctx);

    task_t *task = task_from_lua(L);
    SGE_ASSERT(task->trap_type != TRAP_TYPE_INVALID);

    int ret = task->trap_result;
    if (ret < 0)
        luaL_error(L, "trap %d error %d", task->trap_type, ret);

    task->trap_type = TRAP_TYPE_INVALID;
    task->trap_result = 0;

    return ret;
}

int kernel::do_trap(lua_State *L, int type)
{
    SGE_ASSERT(std::this_thread::get_id() == thread().get_id());
    SGE_ASSERT(type != TRAP_TYPE_INVALID);

    task_t *task = task_from_lua(L);
    SGE_ASSERT(task->trap_type == TRAP_TYPE_INVALID);

    task->trap_type = type;
    task->trap_result = TRAP_ERROR_NOT_SET;

    m_task_list_lock.lock();
    sge_list_add_tail(&m_task_list_trapped, &task->node);
    m_task_list_lock.unlock();

    return lua_yieldk(L, 0, 0, &kernel::trap_done);
}

int kernel::trap_foo(lua_State *L)
{
    return 0;
}

int kernel::sys_current(lua_State *L)
{
    lua_pushthread(L);
    return 1;
}

int kernel::sys_task(lua_State *L)
{
    lua_State *T = lua_newthread(L);
    int top = lua_gettop(L);
    if (top != 1)
        luaL_error(L, "one parameter.");

    int type = lua_type(L, 1);
    switch (type) {
    case LUA_TSTRING:
        luaL_loadstring(T, lua_tostring(L, 1));
        break;
    case LUA_TFUNCTION:
        lua_xmove(L, T, 1);
        break;
    default:
        luaL_error(L, "invalid type.");
        break;
    }

    return 1;
}

void kernel::sys_sleep_done(uv_timer_t *p)
{
    task_t *task = (task_t *)(p->data);
    kernel *k = from_task(task);
    std::unique_lock locker(k->m_task_list_lock);
    sge_list_node_unlink(&task->node);
    sge_list_add_tail(&k->m_task_list_ready, &task->node);
}

int kernel::sys_sleep(lua_State *L)
{
    int ms = (int)luaL_checkinteger(L, 1);
    task_t *task = task_from_lua(L);
    kernel *k = from_task(task);

    if (ms < 1) {
        std::unique_lock locker(k->m_task_list_lock);
        sge_list_add_tail(&k->m_task_list_ready, &task->node);
    } else
        uv_timer_start(&task->sleep_timer, sys_sleep_done, ms, 0);

    return lua_yield(L, 0);
}

int kernel::sys_wait(lua_State *L)
{
    return 0;
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
