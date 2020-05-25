/*
 *
 */
#include <stdio.h>

#include <uv.h>
#include <physfs.h>
#include <SDL.h>

#include <lua.h>
#include <lauxlib.h>

#include <sge/object.h>
#include <sge/renderer.h>
#include <sge/physics.h>

static uv_timer_t sge_frame_timer;
static uv_timer_t sge_state_timer;
static uv_prepare_t sge_sched_preparer;
static sge_list_t sge_task_list_ready;
static sge_list_t sge_task_list_sleep;

static void sge_poll_events(void)
{
	SDL_Event evt;

	while (SDL_PollEvent(&evt)) {
		/* TODO */
	}
}

static void sge_frame(uv_timer_t *p)
{
	sge_poll_events();
}

static void sge_state(uv_timer_t *p)
{
}

static void sge_sched(uv_prepare_t *p)
{
}

static int sge_run(lua_State *L)
{
	sge_renderer_start();
	sge_physics_start();

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	sge_physics_stop();
	sge_renderer_stop();

	return 0;
}

void sge_init(lua_State *L)
{
	int ret;

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0)
		luaL_error(L, "Failed to init SDL.");

	lua_newtable(L);

	lua_pushcfunction(L, &sge_run);
	lua_setfield(L, -2, "run");

	sge_object_init(L);
	sge_renderer_init(L);
	sge_physics_init(L);

	lua_setglobal(L, "sge");

	uv_timer_init(uv_default_loop(), &sge_frame_timer);
	uv_timer_start(&sge_frame_timer, &sge_frame, 0, 16);

	uv_timer_init(uv_default_loop(), &sge_state_timer);
	uv_timer_start(&sge_state_timer, &sge_state, 1000, 1000);

	uv_prepare_init(uv_default_loop(), &sge_sched_preparer);
	uv_prepare_start(&sge_sched_preparer, &sge_sched);
}

void sge_shutdown(lua_State *L)
{
	uv_prepare_stop(&sge_sched_preparer);
	uv_timer_stop(&sge_state_timer);
	uv_timer_stop(&sge_frame_timer);

	sge_physics_shutdown();
	sge_renderer_shutdown();
	sge_object_shutdown();

	SDL_Quit();
}

void sge_task_new(lua_State *L, lua_State *L1)
{

}

void sge_task_destroy(lua_State *L, lua_State *L1)
{

}

void sge_task_resume(lua_State *L, int n)
{

}

void sge_task_yield(lua_State *L, int n)
{

}
