//
//
#include <sge/engine.hpp>

SGE_BEGIN

engine::engine(uv_loop_t *loop)
	: m_loop(loop ? loop : uv_default_loop())
	, m_time_scale(1.0f)
	, m_started(false)
{
	SGE_ASSERT(m_loop != NULL);

	uv_timer_init(m_loop, &m_frame_timer);
	uv_timer_init(m_loop, &m_state_timer);

	m_frame_timer.data = this;
	m_state_timer.data = this;
}

engine::~engine(void)
{
	if (m_started)
		stop();
}

bool engine::start(void)
{
	SGE_ASSERT(m_loop != NULL);
	SGE_ASSERT(!m_started);

	if (!init())
		return false;

	uv_timer_start(&m_frame_timer, &engine::frame_cb, 0, 16);
	uv_timer_start(&m_state_timer, &engine::state_cb, 1000, 1000);

	m_base = uv_now(m_loop);
	m_fps_count = 0;
	m_fps = 0;
	m_started = true;

	return true;
}

void engine::stop(void)
{
	SGE_ASSERT(m_started);

	uv_timer_stop(&m_frame_timer);
	uv_timer_stop(&m_state_timer);

	shutdown();

	m_started = false;
}

void engine::feed_event(const SDL_Event &event)
{
	handle_event(event);
}

bool engine::init(void)
{
	m_renderer.init();

	m_scene.init(&m_renderer);
	m_gui.init(&m_renderer);

	return true;
}

void engine::shutdown(void)
{
	m_gui.shutdown();
	m_scene.shutdown();
	m_renderer.shutdown();
}

void engine::update(float elapsed)
{
	m_scene.update(elapsed);
	m_gui.update(elapsed);
}

void engine::draw(void)
{
	m_scene.draw();
	m_gui.draw();
}

void engine::handle_event(const SDL_Event &event)
{
	switch (event.type) {
	default:
		break;
	}

	m_renderer.handle_event(event);
}

void engine::frame(void)
{
	SGE_ASSERT(m_loop != NULL);

	float elapsed = float(uv_now(m_loop) - m_base) * m_time_scale / 1000.0f;

	update(elapsed);
	
	if (m_renderer.begin()) {
		draw();
		m_renderer.end();
	}

	uv_update_time(m_loop);
	m_base = uv_now(m_loop);
	m_fps_count++;
}

void engine::state(void)
{
	m_fps = m_fps_count;
	m_fps_count = 0;

	// printf("fps %d\n", m_fps);
}

void engine::frame_cb(uv_timer_t *p)
{
	((engine *)(p->data))->frame();
}

void engine::state_cb(uv_timer_t *p)
{
	((engine *)(p->data))->state();
}

SGE_END

