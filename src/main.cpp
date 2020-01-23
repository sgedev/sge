//
//
#include <cstdlib>
#include <iostream>

#include <physfs.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <sge.hpp>

static bool running;

static void SDLCALL writeLog(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
	if (category == SDL_LOG_CATEGORY_ERROR)
		std::cerr << "<SGE> " << message << std::endl;
	else
		std::cout << "<SGE> " << message << std::endl;
}

static inline void pollEvents(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			running = false;
			break;
		default:
			SGE::handleEvent(event);
			break;
		}
	}
}

static inline void update(void)
{
	pollEvents();

	static Uint32 last = SDL_GetTicks();
	Uint32 pass = SDL_GetTicks() - last;

	if (pass < 16) {
		SDL_Delay(1);
		return;
	}

	SGE::frame(float(pass) / 1000.0f);
	last = SDL_GetTicks();
}

int main(int argc, char *argv[])
{
	bool bRet;
	int iRet;
	int exitCode = EXIT_FAILURE;

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif
	SDL_LogSetOutputFunction(writeLog, NULL);

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Simple Game Engine %d.%d.%d",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Copyright(C) 2019 sgedev@gmail.com\n");

	SDL_version ver;
	SDL_GetVersion(&ver);
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SDL version: %d.%d.%d", ver.major, ver.minor, ver.patch);

	iRet = SDL_Init(SDL_INIT_EVERYTHING);
	if (iRet < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL [%d].\n", iRet);
		goto end0;
	}

	iRet = PHYSFS_init(argv[0]);
	if (!iRet) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize PhysFS.\n");
		goto end1;
	}

	bRet = SGE::init();
	if (!bRet) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialzie SGE.\n");
		goto end2;
	}

	running = true;

	while (running)
		update();

	exitCode = EXIT_SUCCESS;

	SGE::shutdown();

end2:
	PHYSFS_deinit();

end1:
	SDL_Quit();

end0:
	return exitCode;
}
