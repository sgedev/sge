//
//
#include <sge/gui/context.hpp>

SGE_GUI_BEGIN

context::context(void)
	: m_renderer(NULL)
{
}

context::~context(void)
{
	if (m_renderer != NULL)
		shutdown();
}

bool context::init(renderer::context *p)
{
	SGE_ASSERT(m_renderer == NULL);
	SGE_ASSERT(p != NULL);

	m_renderer = p;

	return true;
}

void context::shutdown(void)
{
	SGE_ASSERT(m_renderer != NULL);

	m_renderer = NULL;
}

void context::update(float elapsed)
{
}

void context::draw(void)
{
}

SGE_GUI_END

