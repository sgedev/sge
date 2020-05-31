//
//
#include <filesystem>

#include <physfs.h>
#include <cx/list.h>

#include <SGE/Game.hpp>
#include <SGE/Client.hpp>
#include <SGE/Server.hpp>
#include <SGE/Lua.hpp>

SGE_LUA_BEGIN

static const char *ObjectType = "sge.Object";

enum RunMode {
	RunModeClient = 0,
	RunModeServer
};

struct ObjectHolder {
	Object *object;
	int ref;
};

typedef SGELuaTask Task;

static lua_State *mainState;
static cx_list_t taskListReady;
static cx_list_t taskListSleep;
static PHYSFS_Context *physfs;

static SGE_INLINE Task *taskFromLua(lua_State *L)
{
	return (Task *)lua_getextraspace(L);
}

static SGE_INLINE lua_State *taskToLua(Task *task)
{
	return (lua_State *)CX_PMOVB(task, LUA_EXTRASPACE);
}

static void taskNewHook(lua_State *L, lua_State *L1)
{
	Task *task = taskFromLua(L1);
	uv_timer_init(uv_default_loop(), &task->sleepTimer);
	cx_list_node_reset(&task->node);
	cx_list_add_tail(&taskListReady, &task->node);
}

static void taskFreeHook(lua_State *L, lua_State *L1)
{
	Task *task = taskFromLua(L1);
	cx_list_node_unlink(&task->node);
	uv_timer_stop(&task->sleepTimer);
}

static void taskResumeHook(lua_State *L, int n)
{
}

static void taskYieldHook(lua_State *L, int n)
{
}

static int taskNew(lua_State *L)
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
		luaL_error(L, "sge.Task excepts a function or string.");
		break;
	}

	return 1;
}

static void taskSleepDone(uv_timer_t *p)
{
	Task *T = (Task *)(p->data);
	cx_list_node_unlink(&T->node);
	cx_list_add_tail(&taskListReady, &T->node);
}

static int taskSleep(lua_State *L)
{
	int ms = (int)luaL_checkinteger(L, 1);
	if (ms > 0) {
		Task *T = taskFromLua(L);
		uv_timer_start(&T->sleepTimer, &taskSleepDone, ms, 0);
	}

	return lua_yield(L, 0);
}

static int mount(lua_State *L)
{
	return 0;
}

static int umount(lua_State *L)
{
	return 0;
}

static void pollEvents(Game *game)
{
	SDL_Event event;

	SGE_ASSERT(game != NULL);

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			uv_stop(uv_default_loop());
			break;
		default:
			game->handleEvent(event);
			break;
		}
	}
}

static void schedule(void)
{
	cx_list_node_t *node;
	Task *T;
	lua_State *L1;
	int ret;
	int nret;

	while (!cx_list_empty(&taskListReady)) {
		node = cx_list_del_head(&taskListReady);
		T = CX_MEMBEROF(node, Task, node);
		L1 = taskToLua(T);
		ret = lua_resume(L1, mainState, 0, &nret);
		if (ret != LUA_OK && ret != LUA_YIELD) {
			logError("LUAERR: %d - %s", ret, lua_tostring(L1, -1));
		}
	}
}

static void prepare(uv_prepare_t *p)
{
	Game *game = (Game *)(p->data);
	pollEvents(game);
	schedule();
}

static int run(lua_State *L)
{
	RunMode runMode = RunModeClient;
	std::string root(std::filesystem::current_path().string());

	switch (lua_gettop(L)) {
	case 2:
		runMode = (RunMode)luaL_checkinteger(L, 2);
	case 1:
		root = luaL_checkstring(L, 1);
	case 0:
		break;
	default:
		luaL_error(L, "sge.run needs parameters.");
		break;
	}

	if (!std::filesystem::is_directory(root)) {
		logError("'%s' is not a directory.\n", root.c_str());
		return 0;
	}

	if (!PHYSFS_setWriteDir(root.c_str())) {
		logError("failed to mount root directory to '%s'.", root.c_str());
		return 0;
	}

	std::unique_ptr<SGE::Game> game;
	switch (runMode) {
	case RunModeClient:
		game.reset(new Client(uv_default_loop()));
		break;
	case RunModeServer:
		game.reset(new Server(uv_default_loop()));
		break;
	default:
		logError("unknown run mode.");
		break;
	}

	if (!game || !game->start()) {
		logError("failed to start game.");
		return 0;
	}

	uv_prepare_t preparer;
	uv_prepare_init(uv_default_loop(), &preparer);
	uv_prepare_start(&preparer, &prepare);
	preparer.data = game.get();

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_prepare_stop(&preparer);
	game->stop();

	return 0;
}

static int objectNew(lua_State *L)
{
	ObjectHolder *obj = (ObjectHolder *)lua_newuserdata(L, sizeof(ObjectHolder));
	if (obj == NULL)
		luaL_error(L, "no enough memory.");

	obj->object = new Object();
	if (obj->object == NULL) {
		lua_pop(L, 1);
		luaL_error(L, "no enough memory.");
	}

	obj->ref = luaL_ref(L, LUA_REGISTRYINDEX);
	if (obj->ref < 0) {
		lua_pop(L, 1);
		luaL_error(L, "failed to make obj ref.");
	}

	luaL_setmetatable(L, ObjectType);

	return 1;
}

static int objectFree(lua_State *L)
{
	ObjectHolder *obj = (ObjectHolder *)luaL_checkudata(L, 1, ObjectType);

	delete obj->object;
	obj->object = NULL;

	luaL_unref(L, LUA_REGISTRYINDEX, obj->ref);
	obj->ref = LUA_NOREF;

	return 0;
}

static int objectSetProperty(lua_State *L)
{
	ObjectHolder *obj = (ObjectHolder *)luaL_checkudata(L, 1, ObjectType);



	return 0;
}

static void initExports(lua_State *L)
{
	// sge.Object metatable

	luaL_newmetatable(L, ObjectType);

	lua_pushcfunction(L, &objectFree);
	lua_setfield(L, -2, "__gc");

	lua_pushcfunction(L, &objectSetProperty);
	lua_setfield(L, -2, "__newindex");

	lua_pop(L, 1);

	// sge.xxx

	lua_newtable(L);

	lua_pushinteger(L, RunModeClient);
	lua_setfield(L, -2, "RUN_MODE_CLIENT");

	lua_pushinteger(L, RunModeServer);
	lua_setfield(L, -2, "RUN_MODE_SERVER");

	lua_pushcfunction(L, &taskNew);
	lua_setfield(L, -2, "Task");

	lua_pushcfunction(L, &taskSleep);
	lua_setfield(L, -2, "sleep");

	lua_pushcfunction(L, &mount);
	lua_setfield(L, -2, "mount");

	lua_pushcfunction(L, &umount);
	lua_setfield(L, -2, "mount");

	lua_pushcfunction(L, &run);
	lua_setfield(L, -2, "run");

	lua_pushcfunction(L, &objectNew);
	lua_setfield(L, -2, "Object");

	lua_setglobal(L, "sge");
}

static void init(lua_State *L)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		luaL_error(L, "failed to init SDL.");

	physfs = PHYSFS_createContext("sge");
	if (physfs == NULL) {
		SDL_Quit();
		luaL_error(L, "failed to init physfs.");
	}

	PHYSFS_makeCurrent(physfs);

	cx_list_reset(&taskListReady);
	cx_list_reset(&taskListSleep);

	initExports(L);

	mainState = L;
}

static void shutdown(lua_State *L)
{
	PHYSFS_DestroyContext(physfs);
	physfs = NULL;

	SDL_Quit();

	mainState = NULL;
}

SGE_LUA_END

extern "C" void sgeLuaInitHook(lua_State * L)
{
	SGE::Lua::init(L);
}

extern "C" void sgeLuaShutdownHook(lua_State * L)
{
	SGE::Lua::shutdown(L);
}

extern "C" void sgeLuaTaskNewHook(lua_State *L, lua_State *L1)
{
	SGE::Lua::taskNewHook(L, L1);
}

extern "C" void sgeLuaTaskFreeHook(lua_State *L, lua_State *L1)
{
	SGE::Lua::taskFreeHook(L, L1);
}

extern "C" void sgeLuaTaskResumeHook(lua_State *L, int n)
{
	SGE::Lua::taskResumeHook(L, n);
}

extern "C" void sgeLuaTaskYieldHook(lua_State *L, int n)
{
	SGE::Lua::taskYieldHook(L, n);
}
