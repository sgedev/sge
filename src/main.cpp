//
//
#include <cstdlib>

#include <uv.h>
#include <SDL.h>
#include <sge.hpp>
#include <argh.h>

struct sdl_initializer {
	int result;
	sdl_initializer(Uint32 flags): result(SDL_Init(flags)) { }
	~sdl_initializer(void) { if (result == 0) SDL_Quit(); }
	operator bool(void) const { return (result == 0); }
};

static void poll_events(uv_timer_t *timer)
{
	SDL_Event evt;

	while (SDL_PollEvent(&evt)) {
		if (evt.type != SDL_QUIT)
			sge::post_event(evt);
		else
			uv_stop(uv_default_loop());
	}
}

int main(int argc, char *argv[])
{
	int ret;
	uv_timer_t poll_events_timer;

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	sdl_initializer sdl(SDL_INIT_EVERYTHING);
	if (!sdl) {
		SGE_LOGE("Failed to init SDL.");
		return EXIT_FAILURE;
	}

	if (!sge::start(uv_default_loop())) {
		SGE_LOGE("Failed to start.");
		return EXIT_FAILURE;
	}

	uv_timer_init(uv_default_loop(), &poll_events_timer);
	uv_timer_start(&poll_events_timer, poll_events, 0, 10);

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_timer_stop(&poll_events_timer);
	sge::stop();

	return EXIT_SUCCESS;
}
