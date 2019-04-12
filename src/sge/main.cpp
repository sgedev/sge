//
//
#include <sge/common.hpp>
#include <sge/db.hpp>
#include <sge/gl.hpp>
#include <sge/gui.hpp>
#include <sge/scene.hpp>
#include <sge/game.hpp>

SGE_BEGIN

static uv_loop_t *loop;
static uv_timer_t frame_timer;
static uv_timer_t state_timer;
static unsigned int fps;
static unsigned int fps_count;
static uint64_t time_base;
static float time_scale;
static float elapsed;

static void handle_event(const SDL_Event &event)
{
	switch (event.type) {
	case SDL_QUIT:
		uv_stop(loop);
		break;
	default:
		break;
	}

	gl::handle_event(event);
	gui::handle_event(event);
}

static void frame(uv_timer_t *timer)
{
	elapsed = float(uv_now(loop) - time_base);
	elapsed *= time_scale;
	elapsed /= 1000.0f;

	SDL_Event event;
	while (SDL_PollEvent(&event))
		handle_event(event);

	game::update();
	scene::update();
	gui::update();

	if (gl::begin()) {
		scene::draw();
		gui::draw();
		gl::end();
	}

	uv_update_time(loop);

	time_base = uv_now(loop);
	fps_count++;
}

static void state(uv_timer_t *timer)
{
	fps = fps_count;
	fps_count = 0;

	printf("fps %d\n", fps);
}

uv_loop_t *get_loop(void)
{
	return loop;
}

float get_elapsed(void)
{
	return elapsed;
}

unsigned int get_fps(void)
{
	return fps;
}

enum result {
	RESULT_EXIT = 0,
	RESULT_RESTART,
	RESULT_ERROR
};

result main(void)
{
	loop = uv_default_loop();

	db::init(NULL);

	gl::init();
	scene::init();
	gui::init();

	uv_timer_init(loop, &frame_timer);
	uv_timer_init(loop, &state_timer);

	uv_timer_start(&frame_timer, &frame, 0, 16);
	uv_timer_start(&state_timer, &state, 1000, 1000);

	time_base = uv_now(loop);
	time_scale = 1.0f;
	fps_count = 0;
	fps = 0;

	uv_run(loop, UV_RUN_DEFAULT);

	uv_timer_stop(&frame_timer);
	uv_timer_stop(&state_timer);

	gui::shutdown();
	scene::shutdown();
	gl::shutdown();

	db::shutdown();

	return RESULT_EXIT;
}

SGE_END

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	for (;;) {
		sge::result ret = sge::main();
		if (ret == sge::RESULT_EXIT)
			break;
		if (ret != sge::RESULT_RESTART)
			break;
	}

	SDL_Quit();

	return 0;
}

