//
//
#include <SDL.h>

#include <sge/engine.hpp>

static void event_poll(uv_timer_t *p)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			uv_stop(uv_default_loop());
			break;
		}

		((sge::engine *)(p->data))->handle_event(event);
	}
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

	sge::engine *engine = new sge::engine(uv_default_loop());
	if (engine == NULL)
		return EXIT_FAILURE;

	uv_timer_t event_poll_timer;
	uv_timer_init(uv_default_loop(), &event_poll_timer);
	event_poll_timer.data = engine;
	uv_timer_start(&event_poll_timer, event_poll, 16, 16);

	engine->start();

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	engine->stop();

	uv_timer_stop(&event_poll_timer);

	delete engine;

	SDL_Quit();

	return 0;
}

