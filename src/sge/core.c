/*
 *
 */
#include <stdio.h>

#include <uv.h>
#include <physfs.h>
#include <SDL.h>

#include <lua.h>
#include <lauxlib.h>

#include <cx/list.h>

#include <sge/object.h>
#include <sge/renderer.h>
#include <sge/physics.h>

static lua_State *sge_lua;
static uv_timer_t sge_frame_timer;
static uv_timer_t sge_state_timer;
static uv_prepare_t sge_sched_preparer;
static cx_list_t sge_task_list_ready;
static cx_list_t sge_task_list_sleep;
static Uint32 sge_last;

static SGE_INLINE sge_task_t *sge_task_from_lua(lua_State *L)
{
	return (sge_task_t *)lua_getextraspace(L);
}

static SGE_INLINE lua_State *sge_task_to_lua(sge_task_t *task)
{
	return (lua_State *)CX_PMOVB(task, LUA_EXTRASPACE);
}

static void sge_handle_event(const SDL_Event *evt)
{
	SGE_ASSERT(evt != NULL);

	sge_renderer_handle_event(evt);
}

static void sge_poll_events(void)
{
	SDL_Event evt;

	while (SDL_PollEvent(&evt)) {
		switch (evt.type) {
		case SDL_QUIT:
			uv_stop(uv_default_loop());
			break;
		default:
			sge_handle_event(&evt);
			break;
		}
	}
}

static void sge_frame(uv_timer_t *p)
{
	Uint32 curr = SDL_GetTicks();
	Uint32 pass = curr - sge_last;
	float elapsed = ((float)pass) / 1000.0f;

	sge_poll_events();
	sge_physics_update(elapsed);
	sge_renderer_draw(elapsed);
}

static void sge_state(uv_timer_t *p)
{
}

static void sge_sched(uv_prepare_t *p)
{
	cx_list_node_t *node;
	sge_task_t *task;
	lua_State *L;
	int ret;
	int nret;

	while (!cx_list_empty(&sge_task_list_ready)) {
		node = cx_list_del_head(&sge_task_list_ready);
		task = CX_MEMBEROF(node, sge_task_t, node);
		L = sge_task_to_lua(task);
		ret = lua_resume(L, sge_lua, 0, &nret);
		if (ret != LUA_OK && ret != LUA_YIELD) {
			SGE_LOGE("LUAERR: %d - %s", ret, lua_tostring(L, -1));
			// TODO vm exit?
		}
	}
}

static int sge_import(lua_State *L)
{
	// TODO import from physfs.
	lua_pushboolean(L, 0);

	return 1;
}

static int sge_run(lua_State *L)
{
	sge_object_init();
	sge_renderer_init();
	sge_physics_init();

	uv_timer_start(&sge_frame_timer, &sge_frame, 0, 16);
	uv_timer_start(&sge_state_timer, &sge_state, 1000, 1000);
	uv_prepare_start(&sge_sched_preparer, &sge_sched);

	sge_last = SDL_GetTicks();

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_prepare_stop(&sge_sched_preparer);
	uv_timer_stop(&sge_state_timer);
	uv_timer_stop(&sge_frame_timer);

	sge_physics_shutdown();
	sge_renderer_shutdown();
	sge_object_shutdown();

	return 0;
}

static void sge_sleep_done(uv_timer_t *p)
{
	sge_task_t *task = (sge_task_t *)(p->data);
	cx_list_node_unlink(&task->node);
	cx_list_add_tail(&sge_task_list_ready, &task->node);
}

static int sge_sleep(lua_State *L)
{
	int ms = (int)luaL_checkinteger(L, 1);
	sge_task_t *task = sge_task_from_lua(L);
	cx_list_node_unlink(&task->node);
	if (ms > 0) {
		uv_timer_start(&task->sleep_timer, &sge_sleep_done, ms, 0);
		cx_list_add_tail(&sge_task_list_sleep, &task->node);
	} else
		cx_list_add_tail(&sge_task_list_ready, &task->node);

	return lua_yield(L, 0);
}

static int sge_mount(lua_State *L)
{
	const char *src = luaL_checkstring(L, 1);
	const char *dst = luaL_checkstring(L, 2);
	lua_pushboolean(L, PHYSFS_mount(src, dst, 0));
	return 1;
}

static int sge_umount(lua_State *L)
{
	const char *dir = luaL_checkstring(L, 1);
	lua_pushboolean(L, PHYSFS_unmount(dir));
	return 1;
}

static int sge_task(lua_State *L)
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

static void sge_export(lua_State *L)
{
	lua_newtable(L);

	lua_pushcfunction(L, &sge_import);
	lua_setfield(L, -2, "import");

	lua_pushcfunction(L, &sge_run);
	lua_setfield(L, -2, "run");

	lua_pushcfunction(L, &sge_sleep);
	lua_setfield(L, -2, "sleep");

	lua_pushcfunction(L, &sge_mount);
	lua_setfield(L, -2, "mount");

	lua_pushcfunction(L, &sge_umount);
	lua_setfield(L, -2, "umount");

	lua_pushcfunction(L, &sge_task);
	lua_setfield(L, -2, "Task");

	sge_object_export(L);
	sge_renderer_export(L);
	sge_physics_export(L);

	lua_setglobal(L, "sge");
}

void sge_init(lua_State *L)
{
	int ret;

	SGE_LOGI("SGE %d.%d.%d  Copyright (C) 2020 sgedev@gmail.com\n",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0)
		luaL_error(L, "Failed to init SDL.");

	ret = PHYSFS_init("sge");
	if (!ret)
		luaL_error(L, "Failed to init PHYSFS.");

	uv_timer_init(uv_default_loop(), &sge_frame_timer);
	uv_timer_init(uv_default_loop(), &sge_state_timer);
	uv_prepare_init(uv_default_loop(), &sge_sched_preparer);

	cx_list_reset(&sge_task_list_ready);
	cx_list_reset(&sge_task_list_sleep);

	sge_export(L);

	sge_lua = L;
}

void sge_shutdown(lua_State *L)
{
	PHYSFS_deinit();
	SDL_Quit();
}

void sge_task_new(lua_State *L, lua_State *L1)
{
	sge_task_t *task = sge_task_from_lua(L1);
	uv_timer_init(uv_default_loop(), &task->sleep_timer);
	task->sleep_timer.data = task;
	cx_list_node_reset(&task->node);
	cx_list_add_tail(&sge_task_list_ready, &task->node);
}

void sge_task_destroy(lua_State *L, lua_State *L1)
{
	sge_task_t *task = sge_task_from_lua(L1);
	cx_list_node_unlink(&task->node);
}

void sge_task_resume(lua_State *L, int n)
{
}

void sge_task_yield(lua_State *L, int n)
{
}
