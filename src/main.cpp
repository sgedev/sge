//
//
#include <SDL.h>
#include <physfs.h>
#include <sge.hpp>
#include <argh.h>

static bool running;
static float elapsed;
static Uint64 performanceFrequency;
static Uint64 performanceCount;
static Uint64 lastCount;

static inline void pollEvents(void)
{
	SDL_Event evt;

	while (SDL_PollEvent(&evt)) {
		switch (evt.type) {
		case SDL_QUIT:
			running = false;
			break;
		default:
			sge::handleEvent(evt);
			break;
		}
	}	
}

static inline void update(void)
{

}

int main(int argc, char *argv[])
{
	int exitCode = EXIT_FAILURE;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s", SDL_GetError());
		goto end0;
	}

	if (!PHYSFS_init(argv[0])) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to initialize PHYSFS: %s", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		goto end1;
	}

	if (!sge::init()) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SGE: %s", sge::lastError());
		goto end2;
	}

	performanceFrequency = SDL_GetPerformanceFrequency();
	performanceCount = SDL_GetPerformanceCounter();
	lastCount = performanceCount;
	running = true;

	while (running) {
		pollEvents();
		Uint64 curr = SDL_GetPerformanceCounter();
		float diff = float(curr - lastCount) / float(performanceFrequency);
		if (diff > 0.01666) {
			sge::update(elapsed);
			lastCount = curr;
		} else
			SDL_Delay(1);
	}

	exitCode = EXIT_SUCCESS;

end3:
	sge::shutdown();

end2:
	PHYSFS_deinit();

end1:
	SDL_Quit();

end0:
	return exitCode;
}

