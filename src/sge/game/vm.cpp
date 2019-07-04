//
//
#include <physfs.h>

#include "vm.hpp"

SGE_GAME_BEGIN

vm::vm(void)
	: m_L(NULL)
	, m_main_task(NULL)
	, m_state(STATE_IDLE)
	, m_parent(NULL)
	, m_prev_sibling(NULL)
	, m_next_sibling(NULL)
	, m_first_child(NULL)
{
}

vm::~vm(void)
{
	unlink();
}

bool vm::start(const char *initrc)
{
	SGE_ASSERT(m_state == STATE_IDLE);
	SGE_ASSERT(initrc != NULL);

	m_state = STATE_INITIALING;
	m_initrc = initrc;

	std::promise<bool> init_result;
	auto init_result_future = init_result.get_future();

	// start vm thread.
	m_thread = std::thread(&vm::tmain, this, &init_result);

	init_result_future.wait();
	if (!init_result_future.get()) {
		m_thread.join();
		m_state = STATE_IDLE;
		return false;
	}

	return true;
}

void vm::stop(void)
{
	if (m_state != STATE_RUNNING)
		return;

	uv_async_send(&m_quit_async);
	if (m_thread.joinable())
		m_thread.join();
}

void vm::update(void)
{
	if (m_state != STATE_RUNNING)
		return;

	if (m_first_child == NULL)
		return;

	// TODO should be run in vm thread or a lock is needed.

	vm *p = m_first_child;
	vm *n = p->m_next_sibling;

	while (p != NULL) {
		n = p->m_next_sibling;
		if (p->m_state == STATE_EXITED)
			delete p;
		else
			p->update();
		p = n;
	}
}

void vm::tmain(std::promise<bool> *init_result)
{
	SGE_ASSERT(init_result != NULL);

	m_L = luaL_newstate();
	if (m_L == NULL) {
		SGE_LOGE("cannot create lua state: not enough memory\n");
		init_result->set_value(false);
		return;
	}

	lua_set_vm(m_L, this);

	lua_pushcfunction(m_L, &vm::pmain);
	lua_pushlightuserdata(m_L, init_result);
	int status = lua_pcall(m_L, 1, 0, 0);
	lua_close(m_L);
	m_main_task = NULL;

	if (status != LUA_OK) {
		// TODO
	}
}

int vm::pmain(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	vm *_this = vm_from_lua(L);
	SGE_ASSERT(_this != NULL);

	std::promise<bool> *init_result = (std::promise<bool> *)lua_touserdata(L, 1);
	SGE_ASSERT(init_result != NULL);

	_this->vmain(init_result);

	return 0;
}

void vm::vmain(std::promise<bool> *init_result)
{
	SGE_ASSERT(m_L != NULL);
	SGE_ASSERT(init_result != NULL);

	xlist_reset(&m_task_list);
	xlist_reset(&m_task_list_sleep);

	luaL_openlibs(m_L);

	init_traps();

	if (uv_loop_init(&m_loop) < 0) {
		SGE_LOGE("Failed to init main loop object.\n");
		init_result->set_value(false);
		return;
	}

	if (!init_main_task()) {
		SGE_LOGE("Failed to load main task.\n");
		uv_loop_close(&m_loop);
		init_result->set_value(false);
		return;
	}

	uv_async_init(&m_loop, &m_quit_async, quit_cb);
	m_quit_async.data = this;

	m_state = STATE_RUNNING;
	init_result->set_value(true);

	while (m_state == STATE_RUNNING) {
		if (xlist_empty(&m_task_list))
			break;
		schedule_tasks();
		uv_run(&m_loop, UV_RUN_ONCE);
	}

	uv_loop_close(&m_loop);

	m_state = STATE_EXITED;
}

bool vm::init_main_task(void)
{
	SGE_ASSERT(m_L != NULL);
	SGE_ASSERT(m_main_task == NULL);

	SGE_LOGI("Load initrc '%s'...\n", m_initrc.c_str());

	PHYSFS_File *fp = PHYSFS_openRead(m_initrc.c_str());
	if (fp == NULL) {
		SGE_LOGE("Failed to open initrc '%s'.\n", m_initrc.c_str());
		return false;
	}

	PHYSFS_sint64 length = PHYSFS_fileLength(fp);
	if (length < 1) {
		SGE_LOGE("Invalid initrc '%s'.\n", m_initrc.c_str());
		return false;
	}

	if (length > (16 * 1024 * 1024)) {
		SGE_LOGE("The size of initrc '%s' is too big.\n", m_initrc.c_str());
		PHYSFS_close(fp);
		return false;
	}

	char *rc = (char *)malloc(length + 2);
	if (rc == NULL) {
		SGE_LOGE("Not enough memory for initrc '%s'.\n", m_initrc.c_str());
		PHYSFS_close(fp);
		return false;
	}

	memset(rc, 0, length + 2);

	PHYSFS_sint64 readlen = PHYSFS_readBytes(fp, rc, length);
	if (readlen != length) {
		SGE_LOGE("Failed to load initrc '%s'.\n", m_initrc.c_str());
		free(rc);
		PHYSFS_close(fp);
		return false;
	}

	lua_State *main_task = lua_newthread(m_L);
	if (main_task == NULL) {
		SGE_LOGE("Faile to create game main task.\n");
		free(rc);
		PHYSFS_close(fp);
		return false;
	}

	int ret = luaL_loadstring(main_task, rc);
	SGE_LOGD("ret %d!\n", ret);
	free(rc);
	PHYSFS_close(fp);

	if (ret) {
		SGE_LOGE("Failed to parse initrc '%s'.\n", m_initrc.c_str());
		return false;
	}

	m_main_task = task_from_lua(main_task);

	return true;
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
	vm *_this = (vm *)(async->data);
	_this->m_state = STATE_EXITING;
	uv_stop(&_this->m_loop);
}

void vm::add_task(lua_State *L)
{
	SGE_ASSERT(this == vm_from_lua(L));

	task_t *task = task_from_lua(L);

	task_set_vm(task, this);

	xlist_node_reset(&task->node);
	xlist_add_tail(&m_task_list, &task->node);
	xlist_reset(&task->wait_list);

	uv_timer_init(&m_loop, &task->sleep_timer);
	task->sleep_timer.data = task;
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

void vm::init_traps(void)
{
	SGE_ASSERT(m_L != NULL);

	lua_newtable(m_L);

	lua_pushcfunction(m_L, &vm::trap_version);
	lua_setfield(m_L, -2, "version");

	lua_pushcfunction(m_L, &vm::trap_task);
	lua_setfield(m_L, -2, "start");

	lua_pushcfunction(m_L, &vm::trap_current);
	lua_setfield(m_L, -2, "current");

	lua_pushcfunction(m_L, &vm::trap_join);
	lua_setfield(m_L, -2, "join");

	lua_pushcfunction(m_L, &vm::trap_sleep);
	lua_setfield(m_L, -2, "sleep");

	lua_pushcfunction(m_L, &vm::trap_execute);
	lua_setfield(m_L, -2, "execute");

	lua_pushcfunction(m_L, &vm::trap_self);
	lua_setfield(m_L, -2, "self");

	lua_pushcfunction(m_L, &vm::trap_wait);
	lua_setfield(m_L, -2, "wait");

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
	luaL_argcheck(L, lua_isfunction(L, 1), 1, "not a function");

	lua_State *L1 = lua_newthread(L);
	if (L1 != NULL) {
		lua_pushvalue(L, 1);
		lua_xmove(L, L1, 1);
	} else
		lua_pushnil(L);

	return 1;
}

int vm::trap_current(lua_State *L)
{
	if (L == vm_from_lua(L)->m_L)
		luaL_error(L, "Not in a thread");
	lua_pushthread(L);
	return 1;
}

int vm::trap_join(lua_State *L)
{
	return 0;
}

void vm::sleep_done(uv_timer_t *timer)
{
	task_t *task = (task_t *)(timer->data);
	SGE_ASSERT(task != NULL);

	vm *pvm = vm_from_task(task);
	SGE_ASSERT(pvm != NULL);
	SGE_ASSERT(xlist_has(&pvm->m_task_list_sleep, &task->node));

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
		uv_timer_start(&task->sleep_timer, &vm::sleep_done, ms, 0);
		xlist_add_tail(&pvm->m_task_list_sleep, &task->node);
	}

	return lua_yield(L, 0);
}

int vm::trap_execute(lua_State *L)
{
	const char *rc = luaL_checkstring(L, 1);
	if (!PHYSFS_exists(rc)) {
		lua_pushnil(L);
		return 1;
	}

	vm *pvm = new vm();
	if (pvm == NULL) {
		lua_pushnil(L);
		return 1;
	}

	if (!pvm->start(rc)) {
		delete pvm;
		lua_pushnil(L);
		return 1;
	}

	pvm->set_parent(vm_from_lua(L));

	lua_pushlightuserdata(L, pvm);
	return 1;
}

int vm::trap_self(lua_State *L)
{
	lua_pushlightuserdata(L, vm_from_lua(L));
	return 1;
}

int vm::trap_wait(lua_State *L)
{
	luaL_argcheck(L, lua_islightuserdata(L, 1), 1, "vm expected");
	task_t *current = task_from_lua(L);
	vm *self = vm_from_lua(L);
	vm *target = (vm *)lua_touserdata(L, 1);
	if (target == self)
		luaL_error(L, "Cannot wait self.\n");
	// TODO
	return lua_yield(L, 0);
}

void vm::unlink(void)
{
	if (m_parent == NULL)
		return;

	if (m_prev_sibling != NULL)
		m_prev_sibling->m_next_sibling = m_next_sibling;
	else
		m_parent->m_first_child = m_next_sibling;

	if (m_next_sibling != NULL)
		m_next_sibling->m_prev_sibling = m_prev_sibling;

	m_parent = NULL;
}

void vm::set_parent(vm *parent)
{
	if (m_parent == parent)
		return;

	unlink();

	m_parent = parent;
	if (m_parent == NULL)
		return;

	m_prev_sibling = NULL;
	m_next_sibling = m_parent->m_first_child;
	m_parent->m_first_child = this;
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

