//
//
#include <sge/game.hpp>

SGE_BEGIN

game::game(uv_loop_t *loop)
	: m_loop(loop)
	, m_fps(0)
	, m_fps_count(0)
	, m_last_time(0)
{
	SGE_ASSERT(m_loop != NULL);

	uv_timer_init(m_loop, &m_frame_timer);
	uv_timer_init(m_loop, &m_state_timer);

	m_frame_timer.data = this;
	m_state_timer.data = this;
}


bool game::start(void)
{
	SGE_ASSERT(m_loop != NULL);

	m_window.create("SGE", 0, 0, 800, 600);
	m_gl_context.create(m_window);
	m_view.create(m_gl_context);

	if (!init())
		return false;

	uv_timer_init(m_loop, &m_frame_timer);
	uv_timer_init(m_loop, &m_state_timer);

	uv_timer_start(&m_frame_timer, &game::frame_cb, 0, 16);
	uv_timer_start(&m_state_timer, &game::state_cb, 1000, 1000);

	m_fps = 0;
	m_fps_count = 0;
	m_last_time = uv_now(m_loop);

	return true;
}

void game::stop(void)
{
	uv_timer_stop(&m_frame_timer);
	uv_timer_stop(&m_state_timer);

	shutdown();
}

bool game::init(void)
{
	return true;
}

void game::shutdown(void)
{
}

void game::handle_event(const SDL_Event &event)
{
	m_window.handle_event(event);
}

void game::update(float elapsed)
{
}

void game::draw(void)
{
	/* show fps */
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("fps", NULL,
		ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps %d", m_fps);
	ImGui::End();
}

void game::frame(void)
{
	SGE_ASSERT(m_loop != NULL);

	int64_t elapsed = uv_now(m_loop) - m_last_time;
	if (elapsed < 0) {
		m_last_time = uv_now(m_loop);
		return;
	}

	update(float(elapsed) / 1000.0f);

	if (m_window.visibled() && m_gl_context.make_current()) {
		draw();
		m_gl_context.swap_buffers();
	}

	m_last_time = uv_now(m_loop);
	m_fps_count++;

	uv_update_time(m_loop);
}

void game::frame_cb(uv_timer_t *timer)
{
	((game *)(timer->data))->frame();
}

void game::state(void)
{
	m_fps = m_fps_count;
	m_fps_count = 0;
}

void game::state_cb(uv_timer_t *timer)
{
	((game *)(timer->data))->state();
}

SGE_END
