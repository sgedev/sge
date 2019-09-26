//
//
#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <GL/gl3w.h>

#include <glex.hpp>
#include <sge.hpp>

#include "game.hpp"

static bool Running;
static Game MainGame;

static void PollEvents(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			Running = false;
			continue;
		}
	}
}

int main(int argc, char *argv[])
{
	int ret;
	int exit_code = EXIT_FAILURE;

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0)
		return EXIT_FAILURE;

	if (!MainGame.Init()) {
		SDL_Quit();
		return EXIT_FAILURE;
	}

	Running = true;
	while (Running) {
		PollEvents();
		MainGame.Frame(16);
		SDL_Delay(16);
	}

	MainGame.Shutdown();
	SDL_Quit();

	return EXIT_SUCCESS;
}

