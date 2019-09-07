//
//
#include <future>
#include <thread>

#include <lua.hpp>
#include <GL/glex.h>

#include <sge/scene.hpp>
#include <sge/game.hpp>

SGE_GAME_BEGIN

static lua_State *s_L;
static std::thread s_thread;
static uv_loop_t s_loop;
static uv_async_t s_quit_async;
static bool s_running;
static state s_state;

trap_fps_func trap_fps;

static void quit_async(uv_async_t* p)
{
	s_running = false;
	uv_stop(&s_loop);
}

static void init_traps(void)
{
	lua_newtable(s_L);

	lua_setglobal(s_L, "sge");
}

static void schedule(void)
{
}

static void gmain(std::promise<bool>* init_result)
{
	SGE_ASSERT(init_result != NULL);

	uv_loop_init(&s_loop);
	uv_async_init(&s_loop, &s_quit_async, &quit_async);
	s_running = true;

	init_traps();

	init_result->set_value(true);

	while (s_running) {
		uv_run(&s_loop, UV_RUN_DEFAULT);
		schedule();
	}

	uv_loop_close(&s_loop);
}

static int pmain(lua_State* L)
{
	auto init_result = (std::promise<bool>*)lua_touserdata(L, 1);

	SGE_ASSERT(init_result != NULL);

	gmain(init_result);

	return 0;
}

static void tmain(std::promise<bool>* init_result)
{
	int ret;

	SGE_ASSERT(s_L == NULL);

	s_L = luaL_newstate();
	if (s_L == NULL) {
		init_result->set_value(false);
		return;
	}

	lua_pushcfunction(s_L, &pmain);
	lua_pushlightuserdata(s_L, init_result);

	ret = lua_pcall(s_L, 2, 1, 0);
	// TODO

	ret = lua_toboolean(s_L, -1);
	// TODO

	lua_close(s_L);
	s_L = NULL;
}

bool init(void)
{
	SGE_ASSERT(s_L == NULL);

	if (glexInit(NULL) < 0)
		return false;

	return true;

	std::promise<bool> init_result;
	s_thread = std::thread(&tmain, &init_result);
	if (!s_thread.joinable())
		return false;

	auto init_done = init_result.get_future();
	init_done.wait();

	if (!init_done.get())
		return false;

	s_state = STATE_LOADING;

	return true;
}

void shutdown(void)
{
	return;

	SGE_ASSERT(s_L != NULL);


	uv_async_send(&s_quit_async);

	if (s_thread.joinable()) {
		SGE_LOGD("waiting for game thread exited...\n");
		s_thread.join();
	}
}

void handle_event(const SDL_Event *event)
{
}

void update(float elapsed)
{
	scene::update(elapsed);
}

void draw(void)
{
	scene::draw();
}

state current_state(void)
{
	return s_state;
}

SGE_GAME_END

extern "C" void sge_game_luaopen(lua_State *L)
{
}

extern "C" void sge_game_luaclose(lua_State *L)
{
}

extern "C" void sge_game_luathread(lua_State *L, lua_State *L1)
{
}

extern "C" void sge_game_luafree(lua_State *L, lua_State *L1)
{
}

extern "C" void sge_game_luaresume(lua_State *L, int n)
{
}

extern "C" void sge_game_luayield(lua_State *L, int n)
{
}
