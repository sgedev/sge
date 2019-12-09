//
//
#include <uv.h>
#include <lua.hpp>

#include <sge/game.hpp>

SGE_GAME_BEGIN

enum LuaInitResult {
	LuaInitNull = 0,
	LuaInitOk,
	LuaInitError
};

static SDL_Thread *luaThread;
static SDL_mutex *luaMutex;
static SDL_cond *luaCond;
static LuaInitResult luaInitResult;
static uv_async_t luaAsync;

namespace LuaThread {
	static lua_State *lua;
	static uv_loop_t loop;
	static bool running;

	static void asyncHandler(uv_async_t *p)
	{
	}

	static void schedule(lua_State *L)
	{
	}

	static int pmain(lua_State *L)
	{
		luaL_openlibs(L);

		uv_loop_init(&loop);

		uv_async_init(&loop, &luaAsync, asyncHandler);

		running = true;
		luaInitResult = LuaInitOk;
		SDL_CondSignal(luaCond);

		while (running) {
			schedule(L);
			uv_run(&loop, UV_RUN_DEFAULT);
		}

		uv_loop_close(&loop);
	}

	static int tmain(void *p)
	{
		SDL_LockMutex(luaMutex);

		lua = luaL_newstate();
		if (lua == NULL) {
			luaInitResult = LuaInitError;
			SDL_CondSignal(luaCond);
			goto unlock;
		}

		lua_pushcfunction(lua, &pmain);
		lua_pcall(lua, 0, 0, 0);
		lua_close(lua);
		lua = NULL;

	unlock:
		SDL_UnlockMutex(luaMutex);
		return 0;
	}
}

bool init(void)
{
	SGE_ASSERT(luaThread == NULL);
	SGE_ASSERT(luaMutex == NULL);
	SGE_ASSERT(luaCond == NULL);

	luaMutex = SDL_CreateMutex();
	if (luaMutex == NULL)
		goto bad0;

	luaCond = SDL_CreateCond();
	if (luaCond == NULL)
		goto bad1;

	SDL_LockMutex(luaMutex);

	luaInitResult = LuaInitNull;

	luaThread = SDL_CreateThread(&LuaThread::tmain, "SGE::Game", NULL);
	if (luaThread == NULL)
		goto bad2;

	while (luaInitResult == LuaInitNull)
		SDL_CondWait(luaCond, luaMutex);

	SDL_UnlockMutex(luaMutex);

	return true;

bad2:
	SDL_UnlockMutex(luaMutex);

bad1:
	SDL_DestroyMutex(luaMutex);
	luaMutex = NULL;

bad0:
	return false;
}

void shutdown(void)
{
	SGE_ASSERT(luaThread != NULL);

	LuaThread::running = false;
	uv_async_send(&luaAsync);

	SDL_WaitThread(luaThread, NULL);
	luaThread = NULL;

	SDL_DestroyCond(luaCond);
	luaCond = NULL;

	SDL_DestroyMutex(luaMutex);
	luaMutex = NULL;
}

SGE_GAME_END

extern "C" void sge_Game_luaOpen(lua_State *L)
{
}

extern "C" void sge_Game_luaClose(lua_State *L)
{
}

extern "C" void sge_Game_luaThread(lua_State *L, lua_State *L1)
{
}

extern "C" void sge_Game_luaFree(lua_State *L, lua_State *L1)
{
}

extern "C" void sge_Game_luaResume(lua_State *L, int n)
{
}

extern "C" void sge_Game_luaYield(lua_State *L, int n)
{
}

