//
//
#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <ttvfs_zip.h>
#include <GL/glex.h>

#include <sge.hpp>

#include "game.hpp"

static bool running;
static Game *mainGame;

static void pollEvents(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			running = false;
			break;
		default:
			mainGame->handleEvent(&event);
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	int ret;
	int exitCode = EXIT_FAILURE;
	ttvfs::Root root;

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0)
		goto end0;

	ret = glexInit(NULL);
	if (ret != GLEX_OK)
		goto end1;

	mainGame = new Game();
	if (mainGame == NULL)
		goto end2;

	root.AddLoader(new ttvfs::DiskLoader);
	root.AddArchiveLoader(new ttvfs::VFSZipArchiveLoader);
	// TODO mount game directory.

	if (!mainGame->init(&root))
		goto end3;

	running = true;
	while (running) {
		pollEvents();
		mainGame->frame(16);
		SDL_Delay(16);
	}

	exitCode = EXIT_SUCCESS;

end3:
	mainGame->shutdown();
	delete mainGame;

end2:
	glexShutdown();

end1:
	SDL_Quit();

end0:
	return exitCode;
}
