//
//
#include <physfs.h>

#include "vm.hpp"

SGE_GAME_BEGIN

typedef sge_game_vm_task_t task_t;

static inline task_t *task_from_lua(lua_State *L)
{
	return (task_t *)lua_getextraspace(L);
}

static inline lua_State *task_to_lua(task_t *task)
{
	return (lua_State *)(((char *)task) + LUA_EXTRASPACE);
}

static inline void task_set_vm(task_t *task, vm *pvm)
{
	task->data = pvm;
}

static inline void lua_set_vm(lua_State *L, vm *pvm)
{
	task_set_vm(task_from_lua(L), pvm);
}

static inline vm *vm_from_task(task_t *task)
{
	return (vm *)(task->data);
}

static inline vm *vm_from_lua(lua_State *L)
{
	return vm_from_task(task_from_lua(L));
}

vm::vm(void)
	: m_L(NULL)
	, m_running(false)
{
	uv_loop_init(&m_loop);
}

vm::~vm(void)
{
}

bool vm::start(const char *init_script)
{
	SGE_ASSERT(init_script != NULL);

	m_running = false;
	m_init_script = init_script;

	xlist_reset(&m_task_list);
	xlist_reset(&m_task_list_sleep);

	std::promise<bool> init_result;
	auto init_result_future = init_result.get_future();

	// start vm thread.
	m_thread = std::thread(&vm::tmain, this, &init_result);

	init_result_future.wait();
	if (!init_result_future.get()) {
		m_thread.join();
		return false;
	}

	return true;
}

void vm::stop(void)
{
	if (m_running) {
		uv_async_send(&m_quit_async);
		if (m_thread.joinable())
			m_thread.join();
	}
}

void vm::update(void)
{
}

void vm::tmain(std::promise<bool> *init_result)
{
	SGE_ASSERT(init_result != NULL);

	lua_State *L = luaL_newstate();
	if (L == NULL) {
		SGE_LOGE("cannot create state: not enough memory\n");
		init_result->set_value(false);
		return;
	}

	lua_set_vm(L, this);

	lua_pushcfunction(L, &vm::pmain);
	lua_pushlightuserdata(L, this);
	lua_pushlightuserdata(L, init_result);
	int status = lua_pcall(L, 2, 0, 0);
	lua_close(L);

	if (status != LUA_OK) {
		// TODO
	}
}

int vm::pmain(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	vm *_this = (vm *)lua_touserdata(L, 1);
	SGE_ASSERT(_this != NULL);

	std::promise<bool> *init_result = (std::promise<bool> *)lua_touserdata(L, 2);
	SGE_ASSERT(init_result != NULL);

	_this->vmain(init_result);	

	return 0;
}

void vm::vmain(std::promise<bool> *init_result)
{
	SGE_ASSERT(m_L != NULL);
	SGE_ASSERT(init_result != NULL);

	luaL_openlibs(m_L);

	init_traps();

	if (uv_loop_init(&m_loop) < 0) {
		SGE_LOGE("Failed to create game main loop object.\n");
		init_result->set_value(false);
		return;
	}

	lua_State *L = load_main_task();
	if (L == NULL) {
		SGE_LOGE("Failed to create game main task.\n");
		uv_loop_close(&m_loop);
		init_result->set_value(false);
		return;
	}

	uv_async_init(&m_loop, &m_quit_async, quit_cb);
	m_quit_async.data = this;

	init_result->set_value(true);

	m_running = true;
	while (m_running) {
		if (xlist_empty(&m_task_list))
			break;
		schedule_tasks();
		uv_run(&m_loop, UV_RUN_ONCE);
	}

	uv_loop_close(&m_loop);
}

lua_State *vm::load_main_task(void)
{
	SGE_ASSERT(m_L != NULL);

	PHYSFS_File *fp = PHYSFS_openRead(m_init_script.c_str());
	if (fp == NULL) {
		SGE_LOGE("Failed to open init script '%s'.\n", m_init_script.c_str());
		return NULL;
	}

	PHYSFS_sint64 length = PHYSFS_fileLength(fp);
	if (length < 1) {
		SGE_LOGE("Invalid init script '%s'.\n", m_init_script.c_str());
		return NULL;
	}

	char *rc = (char *)malloc(length + 2);
	if (rc == NULL) {
		SGE_LOGE("Not enough memory for '%s'.\n", m_init_script.c_str());
		PHYSFS_close(fp);
		return NULL;
	}

	memset(rc, 0, length + 2);

	PHYSFS_sint64 ret = PHYSFS_readBytes(fp, rc, length);
	if (ret != length) {
		SGE_LOGE("Failed to load init script '%s'.\n", m_init_script.c_str());
		PHYSFS_close(fp);
		return NULL;
	}

	lua_State *main_task = lua_newthread(m_L);
	if (main_task == NULL) {
		SGE_LOGE("Faile to create game main task.\n");
		free(rc);
		PHYSFS_close(fp);
		return NULL;
	}

	luaL_loadstring(main_task, rc);
	free(rc);
	PHYSFS_close(fp);

	return main_task;
}

void vm::add_task(lua_State *L)
{
	SGE_ASSERT(this == vm_from_lua(L));

	task_t *task = task_from_lua(L);
	task_set_vm(task, this);
	xlist_node_reset(&task->node);
	xlist_add_tail(&m_task_list, &task->node);
}

void vm::remove_task(lua_State *L)
{
	SGE_ASSERT(this == vm_from_lua(L));

	task_t *task = task_from_lua(L);
	task_set_vm(task, NULL);
	xlist_node_unlink(&task->node);
}

void vm::resume_task(lua_State *L, int n)
{
	SGE_ASSERT(this == vm_from_lua(L));
}

void vm::yield_task(lua_State *L, int n)
{
	SGE_ASSERT(this == vm_from_lua(L));
}

void vm::schedule_tasks(void)
{
	xlist_node_t *node;
	task_t *task;
	lua_State *L;

	for (;;) {
		node = xlist_del_head(&m_task_list);
		if (node == xlist_knot(&m_task_list))
			break;
		task = SGE_MEMBEROF(node, task_t, node);
		L = task_to_lua(task);
		int ret = lua_resume(L, m_L, 0);
		if (ret != LUA_YIELD && ret != LUA_OK) {
			SGE_LOGD("LUA_ERROR: %s\n", luaL_checkstring(L, -1));
			// TODO
		}
	}
}

void vm::quit_cb(uv_async_t *async)
{
	((vm *)(async->data))->m_running = false;
}

void vm::init_traps(void)
{
	SGE_ASSERT(m_L != NULL);

	lua_newtable(m_L);

	lua_pushcfunction(m_L, &vm::trap_version);
	lua_setfield(m_L, -2, "version");

	lua_pushcfunction(m_L, &vm::trap_task);
	lua_setfield(m_L, -2, "task");

	lua_pushcfunction(m_L, &vm::trap_sleep);
	lua_setfield(m_L, -2, "sleep");

	lua_setglobal(m_L, "sge");
}

int vm::trap_version(lua_State *L)
{
	lua_newtable(L);
	lua_pushinteger(L, SGE_VERSION_MAJOR);
	lua_setfield(L, -2, "major");
	lua_pushinteger(L, SGE_VERSION_MINOR);
	lua_setfield(L, -2, "minor");
	lua_pushinteger(L, SGE_VERSION_PATCH);
	lua_setfield(L, -2, "patch");

	return 1;
}

int vm::trap_task(lua_State *L)
{
	luaL_argcheck(L, lua_isfunction(L, 1), 1, "not function");

	lua_State *L1 = lua_newthread(L);
	if (L1 == NULL) {
		lua_pushnil(L);
		return 1;
	}

	lua_pushvalue(L, 1);
	lua_xmove(L, L1, 1);

	return 1;
}

void vm::sleep_done(uv_timer_t *timer)
{
	task_t *task = (task_t *)(timer->data);
	SGE_ASSERT(task != NULL);
	SGE_ASSERT(xlist_has(&m_task_list_sleep, &task->node));

	vm *pvm = vm_from_task(task);
	SGE_ASSERT(pvm != NULL);

	xlist_del(&pvm->m_task_list_sleep, &task->node);
	xlist_add_tail(&pvm->m_task_list, &task->node);
}

int vm::trap_sleep(lua_State *L)
{
	SGE_LOGD("trap sleep...\n");

	int ms = luaL_checkinteger(L, 1);
	if (ms < 0)
		luaL_error(L, "bad arg");

	if (ms > 0) {
		task_t *task = task_from_lua(L);
		SGE_ASSERT(task != NULL);
		vm *pvm = vm_from_task(task);
		SGE_ASSERT(pvm != NULL);
		uv_timer_start(&task->sleep_timer, vm::sleep_done, ms, 0);
		xlist_add_tail(&pvm->m_task_list_sleep, &task->node);
	}

	return lua_yield(L, 0);
}

SGE_GAME_END

extern "C" void sge_game_vm_open(lua_State *L)
{
}

extern "C" void sge_game_vm_close(lua_State *L)
{
}

extern "C" void sge_game_vm_thread(lua_State *L, lua_State *L1)
{
	sge::game::vm_from_lua(L)->add_task(L1);
}

extern "C" void sge_game_vm_free(lua_State *L, lua_State *L1)
{
	sge::game::vm_from_lua(L)->remove_task(L1);
}

extern "C" void sge_game_vm_resume(lua_State *L, int n)
{
	sge::game::vm_from_lua(L)->resume_task(L, n);
}

extern "C" void sge_game_vm_yield(lua_State *L, int n)
{
	sge::game::vm_from_lua(L)->yield_task(L, n);
}

