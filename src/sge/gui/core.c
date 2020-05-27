/*
 *
 */
#include <sge/gui.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

static NVGcontext *sge_gui_nvg;

void sge_gui_export(lua_State *L)
{
	lua_newtable(L);

	lua_setglobal(L, "gui");
}

bool sge_gui_init(void)
{
	SGE_ASSERT(sge_gui_nvg == NULL);
	sge_gui_nvg = nvgCreateGL3(0);

	return false;
}

void sge_gui_shutdown(void)
{
	SGE_ASSERT(sge_gui_nvg != NULL);
	nvgDeleteGL3(sge_gui_nvg);
	sge_gui_nvg = NULL;
}

void sge_gui_handle_event(const SDL_Event *evt)
{
}

void sge_gui_draw(float elapsed)
{
	SGE_ASSERT(sge_gui_nvg != NULL);

	nvgBeginFrame(sge_gui_nvg, 800.0f, 600.0f, 1.0f);

	// TODO

	nvgEndFrame(sge_gui_nvg);
}

