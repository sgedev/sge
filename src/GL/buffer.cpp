//
//
#include <GL/buffer.hpp>

GL_BEGIN

Buffer::Buffer(GLenum target, GLenum usage)
	: m_id(0)
	, m_target(target)
	, m_usage(usage)
	, m_mapped(false)
{
}

Buffer::~Buffer(void)
{
	destroy();
}

bool Buffer::create(const void *data, int size)
{
	GL_ASSERT(m_id == 0);
	GL_ASSERT(size > 0);

	glGenBuffers(1, &m_id);
	if (m_id == 0)
		return false;

	glBindBuffer(m_target, m_id);
	glBufferData(m_target, size, data, m_usage);

	m_size = size;

	return true;
}

void Buffer::destroy(void)
{
	if (m_id > 0)
		glDeleteBuffers(1, &m_id);
}

GL_END

