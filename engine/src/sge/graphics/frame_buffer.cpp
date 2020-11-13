//
//
#include <sge/graphics/frame_buffer.hpp>

SGE_GRAPHICS_BEGIN

frame_buffer::frame_buffer(void):
	m_id(0)
{
}

frame_buffer::~frame_buffer(void)
{
}

void frame_buffer::resize(int width, int height)
{
    surface::resize(width, height);

	// TODO
}

SGE_GRAPHICS_END
