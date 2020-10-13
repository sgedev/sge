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

void kernel::run(uv_loop_t *loop) {
	sge_list_reset(&m_task_list);

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

void kernel::handle_event(const SDL_Event &evt)
{
	core::handle_event(evt);
}

void kernel::task_added(lua_State *L, lua_State *L1)
{
	task_t *main = task_from_lua(L);
	task_t *task = task_from_lua(L1);

    uv_timer_init(main->loop, &task->sleep_timer);
    task->loop = main->loop;
	task->data = main->data;
	task->sleep_timer.data = task;

	sge_list_node_reset(&task->node);
	sge_list_add_tail(&from_task(task)->m_task_list, &task->node);
}

void kernel::task_removed(lua_State *L, lua_State *L1)
{
	task_t *task = task_from_lua(L1);

	uv_timer_stop(&task->sleep_timer);
	uv_close(reinterpret_cast<uv_handle_t *>(&task->sleep_timer), nullptr);

	sge_list_node_unlink(&task->node);
}

void kernel::task_resume(lua_State *L, int n)
{
}

void kernel::task_yield(lua_State *L, int n)
{
}

bool kernel::load_initrc(lua_State *L)
{
	return true;
}

void kernel::kmain(lua_State *L, uv_loop_t *loop)
{
	luaL_openlibs(L);
	init_exports(L);

	task_t *task = task_from_lua(L);
	task->data = this;
	task->loop = loop;

	if (!load_initrc(L)) {
		set_init_result(false);
		return;
	}

	uv_prepare_init(loop, &m_schedule_preparer);
	uv_prepare_start(&m_schedule_preparer, &kernel::schedule);
	m_schedule_preparer.data = L;

	core::run(loop);

	uv_prepare_stop(&m_schedule_preparer);
	uv_close(reinterpret_cast<uv_handle_t *>(&m_schedule_preparer), nullptr);
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

	while (!sge_list_empty(&k->m_task_list)) {
		node = sge_list_del_head(&k->m_task_list);
		task = SGE_MEMBEROF(node, task_t, node);
		T = task_to_lua(task);
		ret = lua_resume(T, L, 0, &nresults);
		/* TODO ret */
	}
}

void kernel::init_exports(lua_State *L)
{
	lua_registration::begin(L);

	lua_registration::function(L, "current", &kernel::trap_current);
	lua_registration::function(L, "task", &kernel::trap_task);
	lua_registration::function(L, "sleep", &kernel::trap_sleep);
	lua_registration::function(L, "wait", &kernel::trap_wait);

	lua_registration::userdata<scene::world>(L, "world");

	lua_registration::end(L);
}

int kernel::trap_current(lua_State *L)
{
    lua_pushthread(L);
    return 1;
}

int kernel::trap_task(lua_State *L)
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

void kernel::trap_sleep_done(uv_timer_t *p)
{
	task_t *task = (task_t *)(p->data);
	kernel *k = from_task(task);
	//std::lock_guard locker(k->m_mutex);
	sge_list_node_unlink(&task->node);
	sge_list_add_tail(&k->m_task_list, &task->node);
}

int kernel::trap_sleep(lua_State *L)
{
	int ms = (int)luaL_checkinteger(L, 1);
	task_t *task = task_from_lua(L);
	kernel *k = from_task(task);

	if (ms > 0)
		uv_timer_start(&task->sleep_timer, trap_sleep_done, ms, 0);
	else
		sge_list_add_tail(&k->m_task_list, &task->node);

	return lua_yield(L, 0);
}

int kernel::trap_wait(lua_State *L)
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
