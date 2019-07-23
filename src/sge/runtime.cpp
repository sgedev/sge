//
//
#include <sge/runtime.hpp>

SGE_BEGIN

runtime::runtime(void)
	: m_L(NULL)
{
}

runtime::~runtime(void)
{
	if (m_L != NULL)
		shutdown();
}

bool runtime::init(void)
{
	SGE_ASSERT(m_L == NULL);

	m_input.init();

	return true;
}

void runtime::shutdown(void)
{
	SGE_ASSERT(m_L != NULL);

	m_input.shutdown();
}

void runtime::handle_event(const SDL_Event &event)
{
	m_input.handle_event(event);
}

void runtime::update(float elapsed)
{
}

SGE_END

