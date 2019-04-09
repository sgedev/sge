//
//
#include <sge/engine.hpp>

SGE_BEGIN

engine::engine(uv_loop_t *loop)
	: m_loop(loop ? loop : uv_default_loop())
	, m_elapsed(0.0f)
	, m_time_scale(1.0f)
{
	SGE_ASSERT(m_loop != NULL);

	uv_timer_init(m_loop, &m_frame_timer);
	uv_timer_init(m_loop, &m_state_timer);

	m_frame_timer.data = this;
	m_state_timer.data = this;
}

engine::~engine(void)
{
}

bool engine::start(void)
{
	preinit();

	m_renderer.init();
	m_scene.init();
	m_gui.init();

	init();

	uv_timer_start(&m_frame_timer, &engine::frame_cb, 0, 16);
	uv_timer_start(&m_state_timer, &engine::state_cb, 1000, 1000);

	return true;
}

void engine::stop(void)
{
	uv_timer_stop(&m_frame_timer);
	uv_timer_stop(&m_state_timer);

	shutdown();

	m_gui.shutdown();
	m_scene.shutdown();
	m_renderer.shutdown();

	postshutdown();
}

void engine::feed_event(const SDL_Event &event)
{
	handle_event(event);

	switch (event.type) {
	default:
		break;
	}

	m_renderer.handle_event(event);
}

bool engine::preinit(void)
{
	return true;
}

bool engine::init(void)
{
	return true;
}

void engine::shutdown(void)
{
}

void engine::postshutdown(void)
{
}

void engine::update(float elapsed)
{
}

void engine::draw(renderer::context &r)
{
}

void engine::handle_event(const SDL_Event &event)
{
}

void engine::frame(void)
{
	m_elapsed = 0.0f * m_time_scale;

	update(m_elapsed);
	
	m_scene.update(m_elapsed);
	m_gui.update(m_elapsed);

	if (m_renderer.begin()) {
		draw(m_renderer);
		m_scene.draw(m_renderer);
		m_gui.draw(m_renderer);
		m_renderer.end();
	}
}

void engine::state(void)
{
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

