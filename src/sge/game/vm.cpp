//
//
#include <sge/game/vm.hpp>

SGE_GAME_BEGIN

vm::vm(void)
	: m_L(NULL)
	, m_running(false)
	, m_parent(NULL)
	, m_prev(NULL)
	, m_next(NULL)
	, m_first_child(NULL)
{
	xlist_reset(&m_task_list);
}

vm::~vm(void)
{
	stop();
}

bool vm::start(db::blob_ptr initrc)
{
	m_initrc = initrc;

	std::promise<bool> init_result;
	auto init_result_future = init_result.get_future();

	m_thread = std::thread(&vm::tmain, this, &init_result);
	init_result_future.wait();

	return init_result_future.get();
}

void vm::stop(void)
{
	if (!m_running)
		return;

	uv_async_send(&m_quit_async);
	m_thread.join();
}

void vm::poll(void)
{
	if (!m_running)
		return;

}

void vm::init(lua_State *L)
{
	task_t *task = task_from_lua(L);

	xlist_node_reset(&task->node);
	task->vm = NULL;
}

void vm::shutdown(lua_State *L)
{
	task_t *task = task_from_lua(L);

	SGE_ASSERT(!xlist_node_linked(&task->node));
	SGE_ASSERT(task->vm != NULL);
}

void vm::add_task(lua_State *L, lua_State *L1)
{
	vm *_this = from_lua(L);
	task_t *task = task_from_lua(L1);

	SGE_ASSERT(_this != NULL);
	SGE_ASSERT(task != NULL);

	task->vm = (void *)_this;

	xlist_add_tail(&_this->m_task_list, &task->node);
}

void vm::remove_task(lua_State *L, lua_State *L1)
{
	vm *_this = from_lua(L);
	task_t *task = task_from_lua(L1);

	SGE_ASSERT(_this != NULL);
	SGE_ASSERT(task != NULL);
	SGE_ASSERT(task->vm == (void *)_this);

	xlist_node_unlink(&task->node);
}

void vm::resume_task(lua_State *L, int n)
{
	// vm::task_t *task = vm_task_from_lua(L);
}

void vm::yield_task(lua_State *L, int n)
{
	// vm::task_t *task = vm_task_from_lua(L);
}

void vm::schedule_tasks(void)
{
	xlist_node_t *node;
	task_t *task;
	lua_State *L;

	for (;;) {
		node = xlist_del_head(&m_task_list);
		if (node == NULL)
			break;
		task = SGE_MEMBEROF(node, task_t, node);
		L = task_to_lua(task);
		SGE_ASSERT(L != NULL);
		int ret = lua_resume(m_L, L, 0);
		switch (ret) {
		case LUA_YIELD:
			if (!xlist_node_linked(&task->node))
				xlist_add_tail(&m_task_list, &task->node);
			break;
		case LUA_OK:
			break;
		default:
			luaL_error(m_L, "fatal error");
			break;
		}
	}
}

void vm::init_traps(void)
{
	SGE_ASSERT(m_L != NULL);

	luaL_openlibs(m_L);

	lua_newtable(m_L);

	lua_pushcfunction(m_L, &vm::trap_version);
	lua_setfield(m_L, -2, "version");

	lua_pushcfunction(m_L, &vm::trap_task);
	lua_setfield(m_L, -2, "task");

	lua_pushcfunction(m_L, &vm::trap_exec);
	lua_setfield(m_L, -2, "exec");

	lua_pushcfunction(m_L, &vm::trap_kill);
	lua_setfield(m_L, -2, "kill");

	lua_pushcfunction(m_L, &vm::trap_sleep);
	lua_setfield(m_L, -2, "sleep");

	lua_setglobal(m_L, "sge");
}

void vm::quit_cb(uv_async_t *async)
{
	vm *_this = (vm *)(async->data);
	_this->m_running = false;
}

void vm::lmain(std::promise<bool> *init_result)
{
	SGE_ASSERT(m_L != NULL);
	SGE_ASSERT(m_loop == NULL);
	SGE_ASSERT(m_initrc);
	SGE_ASSERT(init_result != NULL);

	init_traps();

	m_loop = uv_loop_new();
	if (m_loop == NULL) {
		init_result->set_value(false);
		return;
	}

	if (!m_initrc->open()) {
		init_result->set_value(false);
		uv_loop_delete(m_loop);
		m_loop = NULL;
		return;
	}

	lua_State *main_task = lua_newthread(m_L);
	if (main_task == NULL) {
		init_result->set_value(false);
		uv_loop_delete(m_loop);
		m_loop = NULL;
	}

	int rc = luaL_loadstring(main_task, (const char *)m_initrc->get_data());
	m_initrc->close();

	if (rc) {
		init_result->set_value(false);
		uv_loop_delete(m_loop);
		m_loop = NULL;
		return;
	}

	m_running = true;
	init_result->set_value(true);

	uv_async_init(m_loop, &m_quit_async, quit_cb);
	m_quit_async.data = this;

	while (m_running) {
		schedule_tasks();
		uv_run(m_loop, UV_RUN_ONCE);
	}

	uv_loop_delete(m_loop);
	m_loop = NULL;
}

int vm::pmain(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	vm *pvm = (vm *)lua_touserdata(L, 1);
	SGE_ASSERT(pvm != NULL);
	SGE_ASSERT(pvm->m_L == L);

	std::promise<bool> *init_result = (std::promise<bool> *)lua_touserdata(L, 2);
	SGE_ASSERT(init_result != NULL);
	pvm->lmain(init_result);

	return 0;
}

void vm::tmain(std::promise<bool> *init_result)
{
	SGE_ASSERT(init_result != NULL);

	m_L = luaL_newstate();
	if (m_L == NULL) {
		SGE_LOGE("cannot create state: not enough memory\n");
		init_result->set_value(false);
		return;
	}

	bind_lua(m_L);

	lua_pushcfunction(m_L, &vm::pmain);
	lua_pushlightuserdata(m_L, this);
	lua_pushlightuserdata(m_L, init_result);
	int status = lua_pcall(m_L, 2, 0, 0);
	lua_close(m_L);

	if (status != LUA_OK) {
		// TODO
	}
}

void vm::unlink(void)
{
	if (m_prev == NULL) {
		if (m_parent != NULL) {
			SGE_ASSERT(m_parent->m_first_child == this);
			m_parent->m_first_child = m_next;
		}
	} else {
		SGE_ASSERT(m_parent != NULL);
		m_prev->m_next = m_next;
	}

	if (m_next != NULL) {
		SGE_ASSERT(m_parent != NULL);
		m_next->m_prev = m_prev;
	}

	m_prev = NULL;
	m_next = NULL;
}

int vm::trap_version(lua_State *L)
{
}

int vm::trap_task(lua_State *L)
{
}

int vm::trap_exec(lua_State *L)
{
}

int vm::trap_kill(lua_State *L)
{
}

int vm::trap_sleep(lua_State *L)
{
}

SGE_GAME_END

extern "C" void sge_game_vm_init(lua_State *L)
{
	sge::game::vm::init(L);
}

extern "C" void sge_game_vm_shutdown(lua_State *L)
{
	sge::game::vm::shutdown(L);
}

extern "C" void sge_game_vm_add_task(lua_State *L, lua_State *L1)
{
	sge::game::vm::add_task(L, L1);
}

extern "C" void sge_game_vm_remove_task(lua_State *L, lua_State *L1)
{
	sge::game::vm::remove_task(L, L1);
}

extern "C" void sge_game_vm_resume_task(lua_State *L, int n)
{
	sge::game::vm::resume_task(L, n);
}

extern "C" void sge_game_vm_yield_task(lua_State *L, int n)
{
	sge::game::vm::yield_task(L, n);
}

