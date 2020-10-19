//
//
#include <sge/graphics/renderer.hpp>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

SGE_GRAPHICS_BEGIN

renderer::renderer(void):
    m_target(nullptr)
{
}

renderer::~renderer(void)
{
    shutdown();
}

bool renderer::init(canvas *target)
{
    SGE_ASSERT(m_target == nullptr);
    SGE_ASSERT(target != nullptr);

    if (!target->begin())
		return false;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    target->end();

    m_target = target;

	return true;
}

void renderer::shutdown(void)
{
    if (m_target == nullptr)
        return;

    m_target->begin();

    // TODO

    m_target->end();

    m_target = nullptr;
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
    glViewport(0, 0, m_target->width(), m_target->height());
	glClear(GL_COLOR_BUFFER_BIT);

	m_target->end();
}

SGE_GRAPHICS_END
