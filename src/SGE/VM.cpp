//
//
#include <chrono>
#include <sstream>
#include <utility>

#include <physfs.h>

#include <SGE/VM.hpp>

SGE_BEGIN

VM::VM(void):
	m_run(false)
{
	std::stringstream ss;
	ss << "/sge/vm/" << (uintptr_t)this;
	ss >> m_root;
}

VM::~VM(void)
{
	stop();
}

bool VM::start(const std::string &initrc)
{
	std::promise<bool> initResult;
	std::future<bool> initFuture = initResult.get_future();

	m_thread = std::thread(&VM::threadMain, this, initrc, &initResult);
	initFuture.wait();

	return initFuture.get();
}

void VM::stop(void)
{
	if (m_run) {
		uv_async_send(&m_asyncExit);
		m_thread.join();
	}
}

void VM::poll(void)
{
	if (m_trap == TrapInvalid)
		return;

	std::lock_guard<std::mutex> lock(m_mutex);

	switch (m_trap) {
	case TrapFps:
		m_trapResult = luaFpsTrap(m_trapStack);
		break;
	case TrapMount:
		m_trapResult = luaMountTrap(m_trapStack);
		break;
	default:
		break;
	}
}

void VM::taskAddedHook(lua_State *L, lua_State *L1)
{
	Task *T = taskFromLua(L);
	SGE_ASSERT(T->vm == this);

	Task *T1 = taskFromLua(L1);
	T1->vm = this;

	uv_timer_init(&m_loop, &T1->sleepTimer);
	T1->sleepTimer.data = T1;

	cx_list_node_reset(&T1->node);
	cx_list_add_tail(&m_taskListReady, &T1->node);
}

void VM::taskRemovedHook(lua_State *L, lua_State *L1)
{
	Task *T = taskFromLua(L);
	SGE_ASSERT(T->vm == this);

	Task *T1 = taskFromLua(L1);
	if (m_current == T1)
		m_current = NULL;
}

void VM::taskResumeHook(lua_State *L, int n)
{
	Task *T = taskFromLua(L);
	SGE_ASSERT(T->vm == this);
	m_current = T;
}

void VM::taskYieldHook(lua_State *L, int n)
{
	Task *T = taskFromLua(L);
	SGE_ASSERT(T->vm == this);
	SGE_ASSERT(m_current == T);
	cx_list_node_unlink(&T->node);
	cx_list_add_tail(&m_taskListSleep, &T->node);
	m_current = NULL;
}

int VM::trapFps(void)
{
	return -1;
}

void VM::threadMain(const std::string &initrc, std::promise<bool> *initResult)
{
	SGE_ASSERT(initResult != NULL);

	std::lock_guard<std::mutex> lock(m_mutex);

	cx_list_reset(&m_taskListReady);
	cx_list_reset(&m_taskListSleep);

	m_current = NULL;
	m_trap = TrapInvalid;
	m_trapStack = NULL;
	m_trapResult = 0;

	lua_State *L = luaL_newstate();
	if (L == NULL) {
		initResult->set_value(false);
		return;
	}

	taskFromLua(L)->vm = (void *)this;

	lua_pushcfunction(L, &VM::luaMain);
	lua_pushlightuserdata(L, this);
	lua_pushstring(L, initrc.c_str());
	lua_pushlightuserdata(L, initResult);
	lua_pcall(L, 3, 0, 0);
	lua_close(L);
}

int VM::luaMain(lua_State *L)
{
	VM *vm = (VM *)lua_touserdata(L, 1);
	const char *initrc = lua_tostring(L, 2);
	std::promise<bool> *initResult = (std::promise<bool> *)lua_touserdata(L, 3);
	vm->vmMain(L, initrc, *initResult);
}

void VM::vmMain(lua_State *L, const char *initrc, std::promise<bool> &initResult)
{
	int ret;
	
	ret = uv_loop_init(&m_loop);
	if (ret < 0) {
		SGE_LOGE("Failed to init vm main loop.");
		initResult.set_value(false);
		return;
	}

	initExports(L);
	
	if (!loadInitTask(L, initrc)) {
		uv_loop_close(&m_loop);
		initResult.set_value(false);
		return;
	}

	uv_async_init(&m_loop, &m_asyncExit, asyncExit);
	m_asyncExit.data = this;

	SGE_LOGI("VM running...");
	m_run = true;
	initResult.set_value(true);

	while (m_run) {
		schedule(L);
		uv_run(&m_loop, UV_RUN_ONCE);
	}

	uv_loop_close(&m_loop);
	m_run = false;

	SGE_LOGI("VM exited.");
}

void VM::asyncExit(uv_async_t *p)
{
	VM *vm = (VM *)(p->data);
	vm->m_run = false;
	uv_stop(p->loop);
}

void VM::initExports(lua_State *L)
{
	luaL_openlibs(L);

	lua_newtable(L);

	lua_pushcfunction(L, &VM::luaTask);
	lua_setfield(L, -2, "Task");

	lua_pushcfunction(L, &VM::luaSleep);
	lua_setfield(L, -2, "sleep");

	lua_pushcfunction(L, &VM::luaFps);
	lua_setfield(L, -2, "fps");

	lua_pushcfunction(L, &VM::luaMount);
	lua_setfield(L, -2, "mount");

	lua_setglobal(L, "sge");
}

bool VM::loadInitTask(lua_State *L, const char *initrc)
{
	SGE_ASSERT(initrc != NULL);

	SGE_LOGI("Loading %s...", initrc);

	lua_State *initTask = lua_newthread(L);
	SGE_LOGD("1 %p", initTask);
	if (initTask == NULL) {
		SGE_LOGE("Failed to create init task.");
		return false;
	}

	int ret = luaL_loadfile(initTask, initrc);
	if (ret != LUA_OK) {
		lua_pop(L, 1);
		SGE_LOGE("Failed to load init task.");
		return false;
	}

	return true;
}

void VM::schedule(lua_State *L)
{
	cx_list_node_t *node;
	Task *T;
	lua_State *L1;
	int ret;
	int nret;

	while (!cx_list_empty(&m_taskListReady)) {
		node = cx_list_del_head(&m_taskListReady);
		T = CX_MEMBEROF(node, Task, node);
		L1 = taskToLua(T);
		ret = lua_resume(L1, L, 0, &nret);
		if (ret != LUA_OK && ret != LUA_YIELD) {
			SGE_LOGE("LUAERR: %d - %s", ret, lua_tostring(L1, -1));
			// TODO vm exit?
		}
	}
}

int VM::doTrap(lua_State *L, Trap trap)
{
	SGE_ASSERT(L != NULL);
	SGE_ASSERT(trap != TrapInvalid);

	SGE_ASSERT(std::this_thread::get_id() == m_thread.get_id());
	SGE_ASSERT(m_trap == TrapInvalid);
	SGE_ASSERT(m_trapStack == NULL);

	m_trap = trap;
	m_trapStack = L;
	m_trapResult = 0;

	m_cond.wait(m_mutex);

	int ret = m_trapResult;

	m_trap = TrapInvalid;
	m_trapStack = NULL;
	m_trapResult = 0;

	if (ret < 0)
		lua_error(L);

	return ret;
}

int VM::luaFps(lua_State *L)
{
	return fromLua(L)->doTrap(L, TrapFps);
}

int VM::luaFpsTrap(lua_State *L)
{
	lua_pushinteger(L, trapFps());
	return 1;
}

int VM::luaMount(lua_State *L)
{
	const char *src = luaL_checkstring(L, 1);
	const char *dst = luaL_checkstring(L, 2);
	return fromLua(L)->doTrap(L, TrapMount);
}

int VM::luaMountTrap(lua_State *L)
{
	const char *src = lua_tostring(L, 1);
	const char *dst = lua_tostring(L, 2);
	std::string mountPoint = fromLua(L)->m_root + '/' + dst;
	lua_pushboolean(L, PHYSFS_mount(src, mountPoint.c_str(), 0));
	return 1;
}

int VM::luaTask(lua_State *L)
{
	int type;
	lua_State *L1;
	
	type = lua_type(L, 1);
	switch (type) {
	case LUA_TFUNCTION:
		L1 = lua_newthread(L);
		if (L1 != NULL) {
			lua_pushvalue(L, 1);
			lua_xmove(L, L1, 1);
		} else
			lua_pushnil(L);
		break;
	case LUA_TSTRING:
		L1 = lua_newthread(L);
		if (L1 != NULL)
			luaL_loadfile(L1, lua_tostring(L, 1));
		else
			lua_pushnil(L1);
		break;
	default:
		luaL_error(L, "unknown type.");
		break;
	}

	return 1;
}

void VM::luaSleepDone(uv_timer_t *p)
{
	Task *T = (Task *)(p->data);
	VM *vm = fromTask(T);
	cx_list_node_unlink(&T->node);
	cx_list_add_tail(&vm->m_taskListReady, &T->node);
}

int VM::luaSleep(lua_State *L)
{
	int ms = luaL_checkinteger(L, 1);
	if (ms > 0) {
		Task *T = taskFromLua(L);
		uv_timer_start(&T->sleepTimer, &VM::luaSleepDone, ms, 0);
	}

	return lua_yield(L, 0);
}

SGE_END

extern "C" void sgeVMTaskAddedHook(lua_State *L, lua_State *L1)
{
	SGE::VM::fromLua(L)->taskAddedHook(L, L1);
}

extern "C" void sgeVMTaskRemovedHook(lua_State *L, lua_State *L1)
{
	SGE::VM::fromLua(L)->taskRemovedHook(L, L1);
}

extern "C" void sgeVMTaskResumeHook(lua_State *L, int n)
{
	SGE::VM::fromLua(L)->taskResumeHook(L, n);
}

extern "C" void sgeVMTaskYieldHook(lua_State *L, int n)
{
	SGE::VM::fromLua(L)->taskYieldHook(L, n);
}
