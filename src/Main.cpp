//
//
#include <stdlib.h>

#include <argh.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "MainWindow.hpp"

static MainWindow mainWindow;
static bool run;

static void pollEvents(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			run = false;
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	int ret;
	Uint32 base;
	Uint32 curr;
	Uint32 pass;

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0)
		return EXIT_FAILURE;

	mainWindow.init("SGE", 0, 0, 800, 600);
	mainWindow.show();

	run = true;
	base = SDL_GetTicks();

	while (run) {
		pollEvents();

		curr = SDL_GetTicks();
		pass = curr - base;
		if (pass < 16) {
			continue;
		}
	}

	SDL_Quit();

	return EXIT_SUCCESS;
}
