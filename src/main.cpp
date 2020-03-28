//
//
#include <cstdlib>

#include <argh.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <SGL/renderer.hpp>

#include "editor.hpp"

static SDL_Window *mainWindow;
static SDL_GLContext glContext;

static int launcherMain(int argc, char *argv[])
{
	int ret;
	int exitCode = EXIT_FAILURE;
	SGL::Renderer::Context *renderer;
	bool run = true;
	SDL_Event event;

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0)
		goto end0;

	mainWindow = SDL_CreateWindow("SGE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
	if (mainWindow == NULL)
		goto end1;

	glContext = SDL_GL_CreateContext(mainWindow);
	if (glContext == NULL)
		goto end2;

	ret = SDL_GL_MakeCurrent(mainWindow, glContext);
	if (ret < 0)
		goto end3;

	renderer = new SGL::Renderer::Context();
	if (renderer == NULL)
		goto end4;

	if (!renderer->init())
		goto end5;

	while (run) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				run = false;
				break;
			}
		}
		ret = SDL_GL_MakeCurrent(mainWindow, glContext);
		if (ret == 0) {
			// TODO
			SDL_GL_SwapWindow(mainWindow);
		}
	}

	exitCode = EXIT_SUCCESS;

end5:
	delete renderer;

end4:
	SDL_GL_MakeCurrent(mainWindow, NULL);

end3:
	SDL_GL_DeleteContext(glContext);

end2:
	SDL_DestroyWindow(mainWindow);

end1:
	SDL_Quit();

end0:
	return exitCode;
}

int main(int argc, char *argv[])
{
	return 0;
}
