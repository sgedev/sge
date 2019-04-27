//
//
#include <sge/game.hpp>

SGE_BEGIN

game::game(void)
{
}

game::~game(void)
{
	// TODO
}

bool game::start(uv_loop_t *loop)
{
	SGE_ASSERT(m_loop == NULL);
	SGE_ASSERT(loop != NULL);

	m_loop = loop;

	//db::init(NULL);

	gl::init();
	gl::make_current();

	scene::init();
	gui::init();
	console::init();
	game::init();

	uv_timer_init(loop, &m_frame_timer);
	uv_timer_init(loop, &m_state_timer);

	uv_timer_start(&m_frame_timer, frame, 0, 16);
	uv_timer_start(&m_state_timer, state, 1000, 1000);

	m_fps = 0;
	m_fps_count = 0;
	m_last_time = uv_now(m_loop);
	m_elapsed = 0.0f;
	m_gui_mode = true;

	return true;
}

void game::stop(void)
{
	SGE_LOGD("Shutdown...\n");

	uv_timer_stop(&m_frame_timer);
	uv_timer_stop(&m_state_timer);

	game::shutdown();
	console::shutdown();
	gui::shutdown();
	scene::shutdown();

	gl::shutdown();

	//db::shutdown();

	SGE_LOGI("Shutdown.\n");
}

void game::handle_event(const SDL_Event &event)
{
	switch (event.type) {
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			s_gui_mode = !s_gui_mode;
			if (s_gui_mode) {
				console::enable();
				SDL_SetRelativeMouseMode(SDL_FALSE);
			} else {
				console::disable();
				SDL_SetRelativeMouseMode(SDL_TRUE);
			}
		}
		break;
	case SDL_QUIT:
		SGE_ASSERT(s_loop != NULL);
		uv_timer_stop(&s_frame_timer);
		uv_timer_stop(&s_state_timer);
		uv_stop(s_loop);
		break;
	default:
		break;
	}
}

void game::update(float elapsed)
{
}

void game::draw(void)
{
}

void game::frame(void)
{
	SDL_Event event;
	int64_t elapsed;

	elapsed = uv_now(s_loop) - s_last_time;
	if (elapsed < 0)
		return;

	update(float(elapsed) / 1000.0f);

	if (gl::make_current()) {
		draw();
		gl::swap_buffers();
	}

	s_last_time = uv_now(s_loop);
	s_fps_count++;

	uv_update_time(s_loop);
}

void game::frame_cb(uv_timer_t *timer)
{
	((game *)(timer->data))->frame();
}

void game::state(void)
{
	s_fps = s_fps_count;
	s_fps_count = 0;
}

void game::state_cb(uv_timer_t *timer)
{
	((game *)(timer->data))->state();
}

SGE_END
