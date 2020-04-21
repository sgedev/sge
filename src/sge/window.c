/*
 *
 */
#include <sge/window.h>

static SDL_Window *SGE_Window;
static SDL_GLContext SGE_GLContext;
static GLEXContext *SGE_GLEXContext;
static NVGcontext *SGE_NVGContext;

int SGE_InitWindow(void)
{
}

void SGE_ShutdownWindow(void)
{
}

int SGE_HandleWindowEvent(const SDL_WindowEvent *event)
{
}

void SGE_RenderWindow(const SGE_WindowDrawer *drawer)
{
}

