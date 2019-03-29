//
//
#include <cstdio>
#include <string>

#include <uv.h>

#include <sge/engine.hpp>

SGE_BEGIN

engine::engine(void)
	: m_started(false)
	, m_drawable(false)
{
}

engine::~engine(void)
{
	if (m_started)
		stop();
}

bool engine::start(uv_loop_t *loop)
{
	if (m_started)
		return false;

	m_loop = loop ? loop : uv_default_loop();
	if (m_loop == NULL)
		return false;

	preinit();

	m_gl_context.init();

	m_frame_last_time = uv_now(m_loop);

	uv_timer_init(m_loop, &m_frame_timer);
	uv_timer_init(m_loop, &m_state_timer);

	m_frame_timer.data = this;
	m_state_timer.data = this;

	uv_timer_start(&m_frame_timer, &engine::frame_cb, 0, 16);
	uv_timer_start(&m_state_timer, &engine::state_cb, 1000, 1000);

	m_gl_context.make_current();

	init();

	m_started = true;

	return true;
}

void engine::stop(void)
{
	SGE_ASSERT(m_started);

	shutdown();

	uv_timer_stop(&m_state_timer);
	uv_timer_stop(&m_frame_timer);

	m_gl_context.shutdown();

	postshutdown();

	m_loop = NULL;
	m_started = false;
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

void engine::draw(void)
{
}

void engine::handle_event(const SDL_Event *event)
{
	SGE_ASSERT(event != NULL);

	switch (event->type) {
	case SDL_WINDOWEVENT:
		handle_window_event(&event->window);
		break;
	case SDL_QUIT:
		uv_stop(m_loop);
		break;
	}
}

void engine::handle_window_event(const SDL_WindowEvent *event)
{
	SGE_ASSERT(event != NULL);

	if (event->windowID != m_gl_context.get_window_id())
		return;

	switch (event->event) {
	case SDL_WINDOWEVENT_EXPOSED:
	case SDL_WINDOWEVENT_SHOWN:
		m_drawable = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
	case SDL_WINDOWEVENT_MINIMIZED:
		m_drawable = false;
		break;
	}
}

void engine::poll_events(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
		handle_event(&event);
}

void engine::frame(void)
{
	uint64_t curr = uv_now(m_loop);
	if (curr <= m_frame_last_time) {
		m_frame_last_time = curr;
		return;
	}

	poll_events();
	update(float(m_frame_last_time - curr) / 1000.0f);

	if (m_drawable) {
		m_gl_context.make_current();
		draw();
		m_gl_context.swap_window();
	}

	uv_update_time(m_loop);

	m_frame_count++;
	m_frame_last_time = uv_now(m_loop);
}

void engine::state(void)
{
	m_frame_count_per_second = m_frame_count;
	m_frame_count = 0;
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

