//
//
#include <GL/buffer.hpp>

GL_BEGIN

Buffer::Buffer(void)
	: m_id(0)
{
}

Buffer::~Buffer(void)
{
	if (m_id > 0)
		glDeleteBuffers(1, &m_id);
}

GL_END
