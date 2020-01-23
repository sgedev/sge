//
//
#include <thread>
#include <mutex>
#include <condition_variable>

#include <lua.hpp>

#include <sge/game.hpp>

SGE_GAME_BEGIN

typedef SGEGameTask VMTask;

enum TrapType {
	TrapTypeNone = 0,
	TrapTypeFps
};

enum TrapError {
	TrapErrorNotSet = -1,
	TrapErrorNotImpl = -2
};

static bool running;
static std::thread vm;
static std::mutex vmMutex;
static std::condition_variable_any vmCond;
static cx_list_t readyTaskList;
static cx_list_t sleepTaskList;
static uv_async_t asyncQuit;

static struct {
	TrapType type;
	lua_State *stack;
	int result;
} trap;

static void initHook(lua_State *L)
{
}

static void shutdownHook(lua_State *L)
{
}

static void addTaskHook(lua_State *L, lua_State *L1)
{
	VMTask *task = SGE_GAME_TASK_FROM_LUA(L1);
	cx_list_node_reset(&task->node);
	cx_list_add_tail(&readyTaskList, &task->node);
}

static void removeTaskHook(lua_State *L, lua_State *L1)
{
	VMTask *task = SGE_GAME_TASK_FROM_LUA(L1);
	cx_list_node_unlink(&task->node);
}

static void resumeTaskHook(lua_State *L, int n)
{
}

static void yieldTaskHook(lua_State *L, int n)
{
}

static inline int doTrap(lua_State *L, TrapType type)
{
	SGE_ASSERT(L != NULL);
	SGE_ASSERT(type != TrapTypeNone);
	SGE_ASSERT(trap.type == TrapTypeNone);

	trap.stack = L;
	trap.result = TrapErrorNotSet;
	trap.type = type;

	vmCond.wait(vmMutex);

	trap.type = TrapTypeNone;
	trap.stack = NULL;

	int ret = trap.result;
	trap.result = -1;

	switch (ret) {
	case TrapErrorNotSet:
		break;
	case TrapErrorNotImpl:
		break;
	}

	return ret;
}

static int trapFps(lua_State *L)
{
	return doTrap(L, TrapTypeFps);
}

static bool initExports(lua_State *L)
{
	lua_newtable(L);

	lua_pushcfunction(L, &trapFps);
	lua_setfield(L, -2, "fps");

	lua_setglobal(L, "sge");

	return true;
}

static bool loadMainTask(lua_State *L)
{
	return true;
}

static void schedule(lua_State *L)
{
	int ret;
	VMTask *task;
	lua_State *L1;
	cx_list_node_t *node;

	while (!cx_list_is_empty(&readyTaskList)) {
		node = cx_list_del_head(&readyTaskList);
		task = CX_MEMBEROF(node, VMTask, node);
		L1 = SGE_GAME_TASK_TO_LUA(task);
		ret = lua_resume(L, L1, 0);
	}
}

static void doQuit(uv_async_t *p)
{
	running = false;
	uv_stop(p->loop);
}

static int luaMain(lua_State *L)
{
	luaL_openlibs(L);

	initExports(L);
	loadMainTask(L);			

	uv_loop_t loop;
	uv_loop_init(&loop);
	uv_async_init(&loop, &asyncQuit, doQuit);

	running = true;
	while (running) {
		schedule(L);
		uv_run(&loop, UV_RUN_ONCE);
	}

	return 0;
}

static void vmMain(void)
{
	std::lock_guard<std::mutex> locker(vmMutex);

	cx_list_reset(&readyTaskList);
	cx_list_reset(&sleepTaskList);

	lua_State *L = luaL_newstate();
	if (L == NULL) {
		logError("Failed to create main lua state.\n");
		return;
	}

	lua_pushcfunction(L, &luaMain);
	lua_pcall(L, 0, 0, 0);

	lua_close(L);
}

///////////////////////////////////////////////////////////////////////////////

static void postQuit(void)
{
	uv_async_send(&asyncQuit);
}

bool init(void)
{
	logInfo("Lua version: " LUA_VERSION_MAJOR "." LUA_VERSION_MINOR "\n");

	vm = std::thread(&vmMain);
	if (!vm.joinable())
		return false;

	return true;
}

void shutdown(void)
{
	postQuit();

	if (vm.joinable())
		vm.join();
}

bool handleEvent(const SDL_Event &event)
{
	return false;
}

void update(float elapsed, const TrapHandler *th)
{
	SGE_ASSERT(th != NULL);

	if (trap.type == TrapTypeNone)
		return;

	std::lock_guard<std::mutex> locker(vmMutex);

	switch (trap.type) {
	case TrapTypeFps:
		lua_pushinteger(trap.stack, th->fps());
		trap.result = 1;
		break;
	default:
		break;
	}
}

SGE_GAME_END

extern "C" void SGEGameInitHook(lua_State *L)
{
	SGE::Game::initHook(L);
}

extern "C" void SGEGameShutdownHook(lua_State *L)
{
	SGE::Game::shutdownHook(L);
}

extern "C" void SGEGameAddTaskHook(lua_State *L, lua_State *L1)
{
	SGE::Game::addTaskHook(L, L1);
}

extern "C" void SGEGameRemoveTaskHook(lua_State *L, lua_State *L1)
{
	SGE::Game::removeTaskHook(L, L1);
}

extern "C" void SGEGameResumeTaskHook(lua_State *L, int n)
{
	SGE::Game::resumeTaskHook(L, n);
}

extern "C" void SGEGameYieldTaskHook(lua_State *L, int n)
{
	SGE::Game::yieldTaskHook(L, n);
}
