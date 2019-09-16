//
//
#include <sge/player.hpp>

SGE_BEGIN

player::player(uv_loop_t *lp)
	: subsystem(lp)
{
	uv_timer_init(loop(), &m_frame_timer);
	uv_timer_init(loop(), &m_state_timer);

	m_frame_timer.data = this;
	m_state_timer.data = this;
}

player::~player(void)
{
}

bool player::init(void)
{
	if (!m_renderer.init())
		return false;

	if (!m_game.init()) {
		m_renderer.shutdown();
		return false;
	}

	m_game.trap_fps = [this](void) { return this->m_fps; };
	m_game.trap_editor_enabled = [](void) { return false; };

	uv_timer_start(&m_frame_timer, &player::frame_cb, 0, 10);
	uv_timer_start(&m_state_timer, &player::state_cb, 1000, 1000);

	uv_update_time(loop());
	m_last = uv_now(loop());

	return true;
}

void player::shutdown(void)
{
	uv_timer_stop(&m_state_timer);
	uv_timer_stop(&m_frame_timer);

	m_game.shutdown();
	m_renderer.shutdown();
}

bool player::handle_event(const SDL_Event *event)
{
	m_renderer.handle_event(event);

	return m_game.handle_event(event);
}

void player::update(float elapsed)
{
	m_game.update(elapsed);
}

void player::draw(void)
{
	m_game.draw();
}

void player::frame_cb(uv_timer_t *p)
{
	SGE_ASSERT(p != NULL);
	SGE_ASSERT(p->data != NULL);

	((player *)(p->data))->frame();
}

void player::frame(void)
{
	uint64_t elapsed = uv_now(loop()) - m_last;

	update(float(elapsed) / 1000.0f);

	if (m_renderer.begin()) {
		draw();
		m_renderer.end();
	}

	m_fps_count += 1;
	uv_update_time(loop());
	m_last = uv_now(loop());
}

void player::state_cb(uv_timer_t *p)
{
	SGE_ASSERT(p != NULL);
	SGE_ASSERT(p->data != NULL);

	((player *)(p->data))->state();
}

void player::state(void)
{
	m_fps = m_fps_count;
	m_fps_count = 0;
}

SGE_END

