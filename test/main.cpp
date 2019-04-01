/*
 *
 */
#include <stdio.h>
#include <sge.hpp>
#include <SDL.h>

static bool run;
static bool drawable;

int main(int argc, char *argv[])
{
	SDL_Window *window;
	SDL_GLContext gl_context;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

#ifdef SGE_DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	window = SDL_CreateWindow("sge-test",
		SDL_WINDOWPOS_CENTERED,	SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (window == NULL)
		return false;

	gl_context = SDL_GL_CreateContext(window);
	if (gl_context == NULL) {
		SDL_DestroyWindow(window);
		return EXIT_FAILURE;
	}

	SDL_GL_MakeCurrent(window, gl_context);
 
	sge::fs fs;
	sge::camera_opengl camera;
	sge::scene scene;

	fs.init("test.zip");
	scene.init(fs, "/scene");
	camera.init();

	run = true;
	while (run) {
		SDL_Delay(16);

		scene.update(0.016f);

		if (drawable) {
			scene.draw(&camera);
			SDL_GL_MakeCurrent(window, gl_context);
			camera.draw();
			SDL_GL_SwapWindow(window);
		}
	}

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return EXIT_SUCCESS;
}

