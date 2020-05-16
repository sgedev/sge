//
//
#include <SGE/Handle.hpp>

SGE_BEGIN

Handle::Handle(uv_loop_t *loop):
	m_loop(loop),
	m_started(false)
{
}

Handle::~Handle(void)
{
	if (m_started)
		stop();
}

bool Handle::start(const std::string &path)
{
	SGE_ASSERT(!m_started);
	m_started = true;
	m_path = path;

	return true;
}

void Handle::stop(void)
{
	SGE_ASSERT(m_started);
	m_started = false;
	m_path.clear();
}

bool Handle::handleEvent(const SDL_Event &event)
{
	return false;
}

SGE_END
