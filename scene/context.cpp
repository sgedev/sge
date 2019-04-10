//
//
#include <sge/scene/context.hpp>

SGE_SCENE_BEGIN

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
}

void context::update(float elapsed)
{
}

void context::draw(void)
{
}

SGE_SCENE_END

