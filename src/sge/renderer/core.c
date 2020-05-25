/*
 *
 */
#include <SDL.h>

#include <sge/renderer.h>

static SDL_Window *sge_renderer_wnd;
static Uint32 sge_renderer_wnd_id;
static SDL_GLContext *sge_renderer_gl;

void sge_renderer_init(lua_State *L)
{
	lua_newtable(L);

	lua_setglobal(L, "renderer");
}

void sge_renderer_shutdown(void)
{

}

bool sge_renderer_start(void)
{
	return true;
}

void sge_renderer_stop(void)
{

}
