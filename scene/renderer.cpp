//
//
#include <algorithm>

#include <sge/scene/renderer.hpp>

SGE_SCENE_BEGIN

renderer::renderer(void)
	: m_began(false)
{
}

renderer::~renderer(void)
{
}

bool renderer::init(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(0.0f);

	m_began = false;

	return true;
}

void renderer::shutdown(void)
{
}

bool renderer::begin(void)
{
	SGE_ASSERT(!m_began);

	m_began = true;

	return true;
}

void renderer::end(void)
{
	SGE_ASSERT(m_began);

	glClear(GL_COLOR_BUFFER_BIT);

	m_began = false;
}

void renderer::draw(drawable *p)
{
	SGE_ASSERT(m_began);
}

SGE_SCENE_END

