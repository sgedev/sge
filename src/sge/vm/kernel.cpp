//
//
#include <sge/vm/kernel.hpp>

SGE_VM_BEGIN

kernel::kernel(uv_loop_t *loop):
	m_loop(loop)
{
	SGE_ASSERT(m_loop != nullptr);

	m_host_async.data = this;
	m_quit_async.data = this;

	sge_list_reset(&m_task_list);
}

kernel::~kernel(void)
{
}

bool kernel::start(const std::string &rootfs, const std::string &initrc)
{
	return true;

	SGE_ASSERT(m_loop != nullptr);

	std::lock_guard locker(m_mutex);

	uv_async_init(m_loop, &m_host_async, &kernel::host);

	m_rootfs = rootfs;
	m_initrc = initrc;
	m_state = STATE_NOT_START;

	m_thread = std::thread([this]() {
		std::lock_guard locker(m_mutex);
		sge_list_reset(&m_task_list);
		lua_State *L = luaL_newstate();
		if (L != nullptr) {
			lua_pushcfunction(L, &kernel::pmain);
			lua_pcall(L, 0, 0, 0);
			lua_close(L);
		} else
			set_state(STATE_ERROR);
	});

	for (;;) {
		m_cond.wait(m_mutex);
		if (m_state == STATE_NOT_START)
			continue;
		if (m_state == STATE_ERROR) {
			m_thread.join();
			return false;
		}
		if (m_state == STATE_RUNNING)
			break;
	}

	return true;
}

void kernel::stop(void)
{
	return;

	uv_async_send(&m_quit_async);
	m_thread.join();

	// TODO
}

void kernel::handle_event(const SDL_Event &evt)
{

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
	sge_list_node_unlink(&task->node);
}

void kernel::task_resume(lua_State *L, int n)
{
}

void kernel::task_yield(lua_State *L, int n)
{
}

void kernel::set_state(state_t st)
{
	if (m_state != st) {
		m_state = st;
		m_cond.notify_one();
	}
}

void kernel::host_run(std::function<void(void)> f)
{
}

void kernel::init_exports(lua_State *L)
{
	lua_newtable(L);

	lua_pushcfunction(L, &kernel::trap_current);
	lua_setfield(L, -2, "current");

	lua_pushcfunction(L, &kernel::trap_task);
	lua_setfield(L, -2, "Task");

	lua_pushcfunction(L, &kernel::trap_sleep);
	lua_setfield(L, -2, "sleep");

	lua_setglobal(L, "sge");
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
	std::lock_guard locker(k->m_mutex);
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

bool kernel::init_rootfs(void)
{
	return true;
}

bool kernel::load_initrc(lua_State *L)
{
	return true;
}

void kernel::frame(uv_timer_t *p)
{
	static int64_t last = uv_now(p->loop);
	int64_t curr = uv_now(p->loop);
	int64_t pass = curr - last;
    if (pass <= 0)
		return;

	kernel *k = from_task(reinterpret_cast<task_t *>(p->data));
	k->m_world.update(float(pass) / 1000.0f);

	k->m_last = curr;
	k->m_frame_count += 1;

	uv_update_time(p->loop);
}

void kernel::count(uv_timer_t *p)
{
	kernel *k = from_task(reinterpret_cast<task_t *>(p->data));
	k->m_frame_per_second = k->m_frame_count;
	k->m_frame_count = 0;
}

void kernel::host(uv_async_t *p)
{
}

void kernel::quit(uv_async_t *p)
{
	reinterpret_cast<kernel *>(p->data)->m_state = STATE_EXITING;
}

void kernel::schedule(lua_State *L)
{
	sge_list_node_t *node;
	task_t *task;
	lua_State *T;
	int ret;
	int nresults;

	while (!sge_list_empty(&m_task_list)) {
		node = sge_list_del_head(&m_task_list);
		task = SGE_MEMBEROF(node, task_t, node);
		T = task_to_lua(task);
		ret = lua_resume(T, L, 0, &nresults);
		/* TODO ret */
	}
}

void kernel::kmain(lua_State *L)
{
    task_t *task;
    uv_loop_t loop;
    uv_timer_t frame_timer;
    uv_timer_t count_timer;

    SGE_LOGI("init='%s' root='%s'", m_initrc.c_str(), m_rootfs.c_str());

	luaL_openlibs(L);
	init_exports(L);

	if (uv_loop_init(&loop) < 0) {
		SGE_LOGE("Failed to init uv loop.");
		set_state(STATE_ERROR);
		return;
	}

    task = task_from_lua(L);
    task->loop = &loop;

	if (!init_rootfs()) {
		set_state(STATE_ERROR);
		return;
	}

	if (!load_initrc(L)) {
		set_state(STATE_ERROR);
		return;
	}

	m_world.init();

    uv_async_init(&loop, &m_quit_async, &quit);

    uv_timer_init(&loop, &frame_timer);
    uv_timer_init(&loop, &count_timer);

	frame_timer.data = this;
	count_timer.data = this;

	uv_timer_start(&frame_timer, &kernel::frame, 0, 16);
    uv_timer_start(&count_timer, &kernel::count, 1000, 1000);

	set_state(STATE_RUNNING);
	while (m_state == STATE_RUNNING) {
		schedule(L);
        m_mutex.unlock();
		uv_run(&loop, UV_RUN_ONCE);
        m_mutex.lock();
	}

    uv_timer_stop(&count_timer);
	uv_timer_stop(&frame_timer);

	uv_loop_close(&loop);
}

int kernel::pmain(lua_State *L)
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
