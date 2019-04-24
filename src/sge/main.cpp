//
//
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
		uv_stop(s_loop);
		break;
	default:
		break;
	}

	gl::handle_event(event);
	gui::handle_event(event);
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

	float elapsed = float(pass) / 1000.0f;

	poll_events();

	scene::update(elapsed);
	gui::update(elapsed);
	game::update(elapsed);

	if (gl::begin()) {
		glClear(GL_COLOR_BUFFER_BIT);
		scene::draw();
		gui::draw();
		gl::end();
	}

	s_last_time = SDL_GetTicks();
	s_fps_count++;
}

static void state(uv_timer_t *timer)
{
	s_fps = s_fps_count;
	s_fps_count = 0;
}

static bool init(uv_loop_t *loop)
{
	SGE_ASSERT(s_loop == NULL);
	SGE_ASSERT(loop != NULL);

	//db::init(NULL);
	gl::init();
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
	s_loop = loop;

	return true;
}

static void shutdown(void)
{
	uv_timer_stop(&s_frame_timer);
	uv_timer_stop(&s_state_timer);

	game::shutdown();
	gui::shutdown();
	scene::shutdown();
	gl::shutdown();
	//db::shutdown();
}

SGE_END

int main(int argc, char *argv[])
{
	int ret;

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0)
		return EXIT_FAILURE;

	sge::init(uv_default_loop());

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	sge::shutdown();

	SDL_Quit();

	return EXIT_SUCCESS;
}

