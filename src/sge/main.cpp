//
//
#define SDL_MAIN_HANDLED

#include <argh.h>
#include <iostream>

#include <sge/common.hpp>
#include <sge/db.hpp>
#include <sge/gl.hpp>
#include <sge/scene.hpp>
#include <sge/gui.hpp>
#include <sge/game.hpp>

SGE_BEGIN

static uv_loop_t *s_loop = NULL;
static uv_timer_t s_frame_timer;
static uv_timer_t s_state_timer;
static unsigned int s_fps = 0;
static unsigned int s_fps_count;
static Uint32 s_last_time;

static void handle_event(const SDL_Event &event)
{
	switch (event.type) {
	case SDL_QUIT:
		SGE_ASSERT(s_loop != NULL);
		uv_timer_stop(&s_frame_timer);
		uv_timer_stop(&s_state_timer);
		uv_stop(s_loop);
		break;
	default:
		break;
	}

	gl::handle_event(event);
	gui::handle_event(event);
	game::handle_event(event);
}

static void poll_events(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
		handle_event(event);
}

static void frame(uv_timer_t *timer)
{
	Uint32 pass = SDL_GetTicks() - s_last_time;
	if (pass < 0)
		return;

	poll_events();

	float elapsed = float(pass) / 1000.0f;

	scene::update(elapsed);
	gui::update(elapsed);
	game::update(elapsed);

	if (gl::make_current()) {
		glClear(GL_COLOR_BUFFER_BIT);
		scene::draw();
		gui::draw();
		gl::swap_buffers();
	}

	s_last_time = SDL_GetTicks();
	s_fps_count++;

	uv_update_time(s_loop);
}

static void state(uv_timer_t *timer)
{
	s_fps = s_fps_count;
	s_fps_count = 0;
}

static void show_info(void)
{
	SGE_LOGI("SGE: %d.%d.%d [%s]\n",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH,
#ifdef SGE_DEBUG
		"DEBUG"
#else
		"RELEASE"
#endif
		);

	SGE_LOGI("Platform: %s, CPUs %d, Memory %dMB\n",
		SDL_GetPlatform(), SDL_GetCPUCount(), SDL_GetSystemRAM());
}

static bool init(uv_loop_t *loop)
{
	SGE_ASSERT(s_loop == NULL);
	SGE_ASSERT(loop != NULL);

	s_loop = loop;

	show_info();

	//db::init(NULL);

	gl::init();
	gl::make_current();

	scene::init();
	gui::init();

	game::init();

	uv_timer_init(loop, &s_frame_timer);
	uv_timer_init(loop, &s_state_timer);

	uv_timer_start(&s_frame_timer, frame, 0, 16);
	uv_timer_start(&s_state_timer, state, 1000, 1000);

	s_fps = 0;
	s_fps_count = 0;
	s_last_time = SDL_GetTicks();

	return true;
}

static void shutdown(void)
{
	SGE_LOGD("Shutdown...\n");

	uv_timer_stop(&s_frame_timer);
	uv_timer_stop(&s_state_timer);

	game::shutdown();

	gui::shutdown();
	scene::shutdown();

	gl::shutdown();

	//db::shutdown();

	SGE_LOGI("Shutdown.\n");
}

uv_loop_t *main_loop(void)
{
	SGE_ASSERT(s_loop != NULL);

	return s_loop;
}

unsigned int fps(void)
{
	return s_fps;
}

SGE_END

struct SDL_Initializer {
	int InitResult;

	SDL_Initializer(void)
	{
		InitResult = SDL_Init(SDL_INIT_EVERYTHING);
	}

	~SDL_Initializer(void)
	{
		if (InitResult == 0)
			SDL_Quit();
	}
};

int main(int argc, char *argv[])
{
	int ret;

	SDL_SetMainReady();

	SDL_Initializer SDL;
	if (SDL.InitResult < 0)
		return EXIT_FAILURE;

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	argh::parser opts(argv);

	if (opts.size() != 2) {
		SGE_LOGE("No game database specified.\n");
		return EXIT_FAILURE;
	}

	if (opts["--version"]) {
        sge::show_info();
		return EXIT_SUCCESS;
	}

	if (opts[{ "E", "--editor" }]) {
		printf("edit mode.\n");
	}

	if (!sge::init(uv_default_loop()))
		return EXIT_FAILURE;

	SGE_LOGI("Running...\n");
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	sge::shutdown();

	SGE_LOGI("Quit.\n");

	return EXIT_SUCCESS;
}

