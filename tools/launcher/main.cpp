//
//
#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <GL/glex.h>

#include <sge.hpp>

#include "game.hpp"

static bool Running;
static Game *MainGame;

static void PollEvents(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			Running = false;
			break;
		default:
			MainGame->handleEvent(&event);
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	int ret;
	int exit_code = EXIT_FAILURE;

	printf("%s(%d)\n", __func__, __LINE__);
	PHYSFS_init(argv[0]);
	printf("%s(%d)\n", __func__, __LINE__);
	glexInit(NULL);
	printf("%s(%d)\n", __func__, __LINE__);

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		PHYSFS_deinit();
		return EXIT_FAILURE;
	}

	printf("%s(%d)\n", __func__, __LINE__);

	MainGame = new Game();

	printf("%s(%d)\n", __func__, __LINE__);

	if (!MainGame->init("/")) {
		SDL_Quit();
		return EXIT_FAILURE;
	}

	printf("%s(%d)\n", __func__, __LINE__);

	Running = true;
	while (Running) {
		PollEvents();
		MainGame->frame(16);
		SDL_Delay(16);
	}

	MainGame->shutdown();
	delete MainGame;

	SDL_Quit();
	glexShutdown();
	PHYSFS_deinit();

	return EXIT_SUCCESS;
}
