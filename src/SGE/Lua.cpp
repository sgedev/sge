//
//
#include <stdio.h>
#include <stdlib.h>

#include <memory>
#include <string>

#include <lua.hpp>
#include <SDL.h>

#include <SGE/Game.hpp>
#include <SGE/Player.hpp>
#include <SGE/Editor.hpp>
#include <SGE/Server.hpp>

enum SGEMode {
	SGE_PLAYER = 0,
	SGE_EDITOR,
	SGE_SERVER
};

static bool sgeRunning;
static SGELuaTask *sgeLuaTaskCurrent;
static cx_list_t sgeLuaTaskListReady;
static cx_list_t sgeLuaTaskListSleep;

SGE_INLINE SGELuaTask *sgeLuaTaskFromLua(lua_State *L)
{
	return (SGELuaTask *)lua_getextraspace(L);
}

SGE_INLINE lua_State *sgeLuaTaskToLua(SGELuaTask *T)
{
	return (lua_State *)CX_PMOVB(T, LUA_EXTRASPACE);
}

static void sgePollEvents(uv_prepare_t *p)
{
	SDL_Event event;

	SGE::Game *game = (SGE::Game *)(p->data);
	SGE_ASSERT(game != NULL);

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			sgeRunning = false;
			uv_stop(p->loop);
			break;
		default:
			game->handleEvent(event);
			break;
		}
	}
}

static void sgeLuaSchedule(lua_State *L)
{
	int ret;
	int nret;
	cx_list_node_t *node;
	SGELuaTask *T;
	lua_State *L1;

	while (!cx_list_empty(&sgeLuaTaskListReady)) {
		node = cx_list_del_head(&sgeLuaTaskListReady);
		T = CX_MEMBEROF(node, SGELuaTask, node);
		L1 = sgeLuaTaskToLua(T);
		ret = lua_resume(L1, L, 0, &nret);
		if (ret != LUA_OK && ret != LUA_YIELD) {
			SGE_LOGE("LUAERR: %d - %s", ret, lua_tostring(L1, -1));
			// TODO vm exit?
		}
	}
}

static int sgeLuaMount(lua_State *L)
{
	const char *src = luaL_checkstring(L, 1);
	const char *dst = luaL_checkstring(L, 2);
	//std::string mountPoint = sgeLuaTaskFromLua(L)->m_root + '/' + dst;
	//lua_pushboolean(L, PHYSFS_mount(src, mountPoint.c_str(), 0));
	return 1;
}

static int sgeLuaRun(lua_State *L)
{
	int mode = SGE_PLAYER;

	switch (lua_gettop(L)) {
	case 1:
		mode = luaL_checkinteger(L, 1);
	case 0:
		break;
	default:
		luaL_error(L, "too many parameters.");
		break;
	}

	std::unique_ptr<SGE::Game> game;

	switch (mode) {
	case SGE_PLAYER:
		game.reset(new SGE::Player(uv_default_loop()));
		break;
	case SGE_EDITOR:
		game.reset(new SGE::Editor(uv_default_loop()));
		break;
	case SGE_SERVER:
		game.reset(new SGE::Server(uv_default_loop()));
		break;
	default:
		luaL_error(L, "invalid mode.");
		break;
	}

	if (!game || !game->start())
		luaL_error(L, "Failed to start game.");

	uv_prepare_t eventPoller;
	uv_prepare_init(uv_default_loop(), &eventPoller);
	uv_prepare_start(&eventPoller, sgePollEvents);
	eventPoller.data = game.get();

	// Main loop.
	sgeRunning = true;
	for (;;) {
		sgeLuaSchedule(L);
		if (!sgeRunning)
			break;
		uv_run(uv_default_loop(), UV_RUN_ONCE);
	}

	game->stop();
	uv_prepare_stop(&eventPoller);
	
	return 0;
}

static void sgeLuaSleepDone(uv_timer_t *p)
{
	SGELuaTask *T = (SGELuaTask *)(p->data);
	cx_list_node_unlink(&T->node);
	cx_list_add_tail(&sgeLuaTaskListReady, &T->node);
}

static int sgeLuaSleep(lua_State *L)
{
	int ms = luaL_checkinteger(L, 1);
	if (ms > 0) {
		SGELuaTask *T = sgeLuaTaskFromLua(L);
		uv_timer_start(&T->sleepTimer, &sgeLuaSleepDone, ms, 0);
	}

	return lua_yield(L, 0);
}

static void sgeLuaInitExports(lua_State *L)
{
	lua_newtable(L);

	lua_pushcfunction(L, &sgeLuaMount);
	lua_setfield(L, -2, "mount");

	lua_pushcfunction(L, &sgeLuaRun);
	lua_setfield(L, -2, "run");

	lua_pushcfunction(L, &sgeLuaSleep);
	lua_setfield(L, -2, "sleep");

	lua_setglobal(L, "sge");
}

static void sgePrintBanner(void)
{
	SGE_LOGI("Simple Game Engine V%d.%d.%d",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);
}

static void sgePrintLog(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
	if (priority == SDL_LOG_PRIORITY_ERROR || priority == SDL_LOG_PRIORITY_CRITICAL)
		fprintf(stderr, "SGE: %s\n", message);
	else
		fprintf(stdout, "SGE: %s\n", message);
}

extern "C" void sgeLuaInit(lua_State *L)
{
	int ret;

	SDL_LogSetOutputFunction(sgePrintLog, NULL);

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	sgePrintBanner();

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret == 0)
		luaL_error(L, "Failed to init SDL.");

	cx_list_reset(&sgeLuaTaskListReady);
	cx_list_reset(&sgeLuaTaskListSleep);

	sgeLuaInitExports(L);

	SGELuaTask *T = sgeLuaTaskFromLua(L);
	uv_timer_init(uv_default_loop(), &T->sleepTimer);
	cx_list_node_reset(&T->node);
	sgeLuaTaskCurrent = T;
}

extern "C" void sgeLuaShutdown(lua_State *L)
{
	SDL_Quit();
}

extern "C" void sgeLuaTaskNew(lua_State *L, lua_State *L1)
{
	SGELuaTask *T1 = sgeLuaTaskFromLua(L1);

	uv_timer_init(uv_default_loop(), &T1->sleepTimer);
	T1->sleepTimer.data = T1;

	cx_list_node_reset(&T1->node);
	cx_list_add_tail(&sgeLuaTaskListReady, &T1->node);
}

extern "C" void sgeLuaTaskFree(lua_State *L, lua_State *L1)
{
	SGELuaTask *T1 = sgeLuaTaskFromLua(L1);
	if (sgeLuaTaskCurrent == T1)
		sgeLuaTaskCurrent = NULL;
	cx_list_node_unlink(&T1->node);
}

extern "C" void sgeLuaTaskResume(lua_State *L, int n)
{
	sgeLuaTaskCurrent = sgeLuaTaskFromLua(L);
}

extern "C" void sgeLuaTaskYield(lua_State *L, int n)
{
	SGELuaTask *T = sgeLuaTaskFromLua(L);
	SGE_ASSERT(sgeLuaTaskCurrent == T);
	cx_list_node_unlink(&T->node);
	cx_list_add_tail(&sgeLuaTaskListSleep, &T->node);
	sgeLuaTaskCurrent = NULL;
}
