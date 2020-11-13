//
//
#include <sge/graphics/surface.hpp>

SGE_GRAPHICS_BEGIN

surface::surface(void)
{
}

surface::~surface(void)
{
}

void surface::resize(int width, int height)
{
	m_size.x = width;
	m_size.y = height;
}

bool surface::begin(void)
{
	return true;
}

void surface::end(void)
{
}

SGE_GRAPHICS_END
