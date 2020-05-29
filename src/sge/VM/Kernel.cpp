//
//
#include <chrono>
#include <sstream>
#include <utility>

#include <SGE/VM/Kernel.hpp>

SGE_VM_BEGIN

Kernel::Kernel(uv_loop_t *trapLoop):
	m_trapLoop(trapLoop),
	m_run(false),
	m_L(NULL)
{
	SGE_ASSERT(m_trapLoop != NULL);
	uv_async_init(m_trapLoop, &m_trapAsync, &Kernel::trapAsync);
	m_trapAsync.data = this;
}

Kernel::~Kernel(void)
{
	stop();
}

bool Kernel::start(const std::string &initrc)
{
	cx_list_reset(&m_taskListReady);
	cx_list_reset(&m_taskListSleep);
	cx_list_reset(&m_taskListTraps);

	std::promise<bool> initResult;
	std::future<bool> initFuture = initResult.get_future();

	m_thread = std::thread(&Kernel::startup, this, initrc, &initResult);

	return initFuture.get();
}

void Kernel::stop(void)
{
	if (m_run) {
		uv_async_send(&m_exitAsync);
		m_thread.join();
	}
}

void Kernel::taskAddedHook(lua_State *L, lua_State *L1)
{
	SGE_ASSERT(inKernel());

	Task *T = taskFromLua(L);
	SGE_ASSERT(T->K == this);

	Task *T1 = taskFromLua(L1);
	T1->K = this;

	uv_timer_init(&m_loop, &T1->sleepTimer);
	T1->sleepTimer.data = T1;

	cx_list_node_reset(&T1->node);
	cx_list_add_tail(&m_taskListReady, &T1->node);
}

void Kernel::taskRemovedHook(lua_State *L, lua_State *L1)
{
	SGE_ASSERT(inKernel());

	Task *T = taskFromLua(L);
	SGE_ASSERT(T->K == this);

	Task *T1 = taskFromLua(L1);
	cx_list_node_unlink(&T1->node);
	uv_timer_stop(&T1->sleepTimer);
}

void Kernel::taskResumeHook(lua_State *L, int n)
{
	SGE_ASSERT(inKernel());
}

void Kernel::taskYieldHook(lua_State *L, int n)
{
	SGE_ASSERT(inKernel());

	Task *T = taskFromLua(L);
	SGE_ASSERT(T->K == this);
}

void Kernel::startup(const std::string &initrc, std::promise<bool> *initResult)
{
	SGE_ASSERT(initResult != NULL);

	std::lock_guard<std::mutex> lock(m_mutex);

	m_L = luaL_newstate();
	if (m_L == NULL) {
		initResult->set_value(false);
		return;
	}

	taskFromLua(m_L)->K = (void *)this;

	lua_pushcfunction(m_L, &Kernel::pmain);
	lua_pushstring(m_L, initrc.c_str());
	lua_pushlightuserdata(m_L, initResult);
	lua_pcall(m_L, 2, 0, 0);
	lua_close(m_L);
}

int Kernel::pmain(lua_State *L)
{
	luaL_openlibs(L);

	Kernel *K = fromLua(L);
	const char *initrc = lua_tostring(L, 1);
	std::promise<bool> *initResult = (std::promise<bool> *)lua_touserdata(L, 2);

	K->kmain(L, initrc, *initResult);

	return 0;
}

void Kernel::kmain(lua_State *L, const char *initrc, std::promise<bool> &initResult)
{
	int ret;

	ret = uv_loop_init(&m_loop);
	if (ret < 0) {
		SGE_LOGE("Failed to init kernel loop.");
		initResult.set_value(false);
		return;
	}

	initExports(m_L);

	if (!init()) {
		uv_loop_close(&m_loop);
		initResult.set_value(false);
		return;
	}
	
	if (!loadInitTask(L, initrc)) {
		shutdown();
		uv_loop_close(&m_loop);
		initResult.set_value(false);
		return;
	}

	uv_async_init(&m_loop, &m_exitAsync, &Kernel::exitAsync);
	m_exitAsync.data = this;

	SGE_LOGI("Kernel running...");
	m_run = true;
	initResult.set_value(true);

	while (m_run) {
		schedule(m_L);
		if (!cx_list_empty(&m_taskListTraps))
			uv_async_send(&m_trapAsync);
		m_mutex.unlock();
		uv_run(&m_loop, UV_RUN_ONCE);
		m_mutex.lock();
		uv_update_time(&m_loop);
	}

	shutdown();

	uv_loop_close(&m_loop);
	m_run = false;

	SGE_LOGI("Kernel exited.");
}

void Kernel::exitAsync(uv_async_t *p)
{
	Kernel *K = (Kernel *)(p->data);
	K->m_run = false;
	uv_stop(p->loop);
}

void Kernel::initExports(lua_State *L)
{
	lua_pushcfunction(L, &Kernel::luaTask);
	lua_setglobal(L, "Task");

	lua_pushcfunction(L, &Kernel::luaSleep);
	lua_setglobal(L, "sleep");

	lua_pushcfunction(L, &Kernel::luaMount);
	lua_setglobal(L, "mount");
}

bool Kernel::loadInitTask(lua_State *L, const char *initrc)
{
	SGE_ASSERT(initrc != NULL);

	SGE_LOGI("Loading %s...", initrc);

	lua_State *initTask = lua_newthread(L);
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

void Kernel::schedule(lua_State *L)
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

void Kernel::trapsHandler(uv_loop_t *p)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	cx_list_node_t *node;
	Task *T;
	Kernel *K = ((Kernel *)(p->data));

	CX_LIST_FOREACH(node, &K->m_taskListTraps) {
		T = CX_MEMBEROF(node, Task, node);
		switch (T->trapType) {
		default:
			T->trapResult = TrapErrorUnknown;
			break;
		}
		cx_list_node_unlink(&T->node);
		cx_list_add_tail(&fromTask(T)->m_taskListReady, &T->node);
	}
}

void Kernel::trapAsync(uv_async_t *p)
{
	((Kernel *)(p->data))->trapsHandler(p->loop);
}

int Kernel::trapDone(lua_State *L, int status, lua_KContext ctx)
{
	Task *T = taskFromLua(L);

	int ret = T->trapResult;
	if (ret < 0)
		lua_error(L);

	T->trapType = TrapTypeInvalid;
	T->trapResult = 0;

	return ret;
}

int Kernel::trap(lua_State *L, TrapType type)
{
	SGE_ASSERT(L != NULL);
	SGE_ASSERT(type != TrapTypeInvalid);

	Task *T = taskFromLua(L);

	SGE_ASSERT(inKernel());
	SGE_ASSERT(T->trapType == TrapTypeInvalid);

	T->trapType = type;
	T->trapResult = 0;

	cx_list_add_tail(&m_taskListTraps, &T->node);

	return lua_yieldk(L, 0, lua_KContext(this), &Kernel::trapDone);
}

int Kernel::luaMount(lua_State *L)
{
	const char *src = luaL_checkstring(L, 1);
	const char *dst = luaL_checkstring(L, 2);
	lua_pushboolean(L, fromLua(L)->m_root.mount(dst, src));
	return 1;
}

int Kernel::luaTask(lua_State *L)
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

void Kernel::luaSleepDone(uv_timer_t *p)
{
	Task *T = (Task *)(p->data);
	Kernel *K = fromTask(T);
	cx_list_node_unlink(&T->node);
	cx_list_add_tail(&K->m_taskListReady, &T->node);
}

int Kernel::luaSleep(lua_State *L)
{
	int ms = (int)luaL_checkinteger(L, 1);
	if (ms > 0) {
		Task *T = taskFromLua(L);
		uv_timer_start(&T->sleepTimer, &Kernel::luaSleepDone, ms, 0);
	}

	return lua_yield(L, 0);
}

SGE_VM_END

extern "C" void sgeVMTaskAddedHook(lua_State *L, lua_State *L1)
{
	SGE::VM::Kernel::fromLua(L)->taskAddedHook(L, L1);
}

extern "C" void sgeVMTaskRemovedHook(lua_State *L, lua_State *L1)
{
	SGE::VM::Kernel::fromLua(L)->taskRemovedHook(L, L1);
}

extern "C" void sgeVMTaskResumeHook(lua_State *L, int n)
{
	SGE::VM::Kernel::fromLua(L)->taskResumeHook(L, n);
}

extern "C" void sgeVMTaskYieldHook(lua_State *L, int n)
{
	SGE::VM::Kernel::fromLua(L)->taskYieldHook(L, n);
}
