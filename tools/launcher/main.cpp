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
		switch (event.type) {
		case SDL_QUIT:
			Running = false;
			break;
		default:
			MainGame.handleEvent(&event);
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return EXIT_FAILURE;

	if (!MainGame.init("/")) {
		SDL_Quit();
		return EXIT_FAILURE;
	}

	Running = true;
	while (Running) {
		PollEvents();
		MainGame.frame(16);
		SDL_Delay(16);
	}

	MainGame.shutdown();
	SDL_Quit();

	return EXIT_SUCCESS;
}
