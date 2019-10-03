//
//
#include <cstdio>

#include <physfs.h>

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

		MainGame.HandleEvent(&event);
	}
}

int main(int argc, char *argv[])
{
	if (!PHYSFS_init(NULL))
		return EXIT_FAILURE;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		PHYSFS_deinit();
		return EXIT_FAILURE;
	}

	if (!MainGame.Init()) {
		SDL_Quit();
		PHYSFS_deinit();
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
	PHYSFS_deinit();

	return EXIT_SUCCESS;
}
