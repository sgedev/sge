//
//
#include <sge/graphics/canvas.hpp>

SGE_GRAPHICS_BEGIN

canvas::canvas(void)
{
}

canvas::~canvas(void)
{
}

void canvas::resize(int width, int height)
{
	m_size.x = width;
	m_size.y = height;
}

bool canvas::begin(void)
{
	return true;
}

void canvas::end(void)
{
}

SGE_GRAPHICS_END
