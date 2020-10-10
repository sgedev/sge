//
//
#include <sge/graphics/renderer.hpp>

SGE_GRAPHICS_BEGIN

renderer::renderer(void):
	m_target(nullptr)
{
}

renderer::~renderer(void)
{
}

void renderer::set_target(canvas *p)
{
	// TODO
	m_target = p;
}

SGE_GRAPHICS_END
