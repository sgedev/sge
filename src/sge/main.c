//
//
#include <stdio.h>
#include <stdlib.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <sge/common.h>
#include <sge/input.h>
#include <sge/window.h>

static int SGE_Run;

static void SGE_Draw3D(void)
{

}

static void SGE_Draw2D(NVGcontext *nvg)
{

}

static void SGE_PollEvents(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_MOUSEMOTION:
			SGE_HandleMouseMoveEvent(&event.motion);
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			SGE_HandleKeyEvent(&event.key);
			break;
		case SDL_WINDOWEVENT:
			SGE_HandleWindowEvent(&event.window);
			break;
		case SDL_QUIT:
			SGE_Run = 0;
			break;
		default:
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	int ret;
	int exitCode = EXIT_FAILURE;

	static const SGE_WindowDrawer windowDrawer = {
		SGE_Draw3D,
		SGE_Draw2D
	};

	ret = SGE_InitWindow();
	if (ret < 0)
		goto end0;

	ret = SGE_InitInput();
	if (ret < 0)
		goto end1;

	SGE_Run = 1;
	while (SGE_Run) {
		SGE_PollEvents();
		SGE_UpdateInput(16.6f);
		SGE_RenderWindow(&windowDrawer);
	}

	SGE_ShutdownInput();

end1:
	SGE_ShutdownWindow();

end0:
	return exitCode;
}

