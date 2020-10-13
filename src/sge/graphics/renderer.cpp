//
//
#include <sge/graphics/renderer.hpp>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

SGE_GRAPHICS_BEGIN

renderer::renderer(canvas *target):
	m_target(target)
{
	SGE_ASSERT(m_target != nullptr);
}

renderer::~renderer(void)
{
}

bool renderer::init(void)
{
	SGE_ASSERT(m_target != nullptr);

	if (!m_target->begin())
		return false;

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	m_target->end();

	return true;
}

void renderer::shutdown(void)
{
}

void renderer::begin(void)
{
	SGE_ASSERT(m_target != nullptr);
}

void renderer::end(void)
{
	SGE_ASSERT(m_target != nullptr);

	if (!m_target->begin())
		return;

	// TODO

	glClear(GL_COLOR_BUFFER_BIT);

	m_target->end();
}

SGE_GRAPHICS_END
