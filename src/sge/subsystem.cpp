//
//
#include <sge/subsystem.hpp>

SGE_BEGIN

subsystem::subsystem(uv_loop_t *lp)
	: m_loop(lp ? lp : uv_default_loop())
	, m_started(false)
{
}

subsystem::~subsystem(void)
{
	if (m_started)
		stop();
}

bool subsystem::start(void)
{
	SGE_ASSERT(m_loop != NULL);
	SGE_ASSERT(!m_started);

	if (!init())
		return false;

	m_started = true;
	return true;
}

void subsystem::stop(void)
{
	SGE_ASSERT(m_loop != NULL);
	SGE_ASSERT(m_started);

	shutdown();

	m_started = false;
}

void subsystem::feed_event(const SDL_Event &event)
{
	SGE_ASSERT(m_loop != NULL);
	SGE_ASSERT(m_started);

	handle_event(event);
}

SGE_END

