//
//
#include <sge/gl/buffer.h>

SGE_GL_BEGIN

Buffer::Buffer(GLenum target, GLenum usage)
	: m_id(0)
	, m_target(target)
	, m_usage(usage)
	, m_mapped(false)
{
}

Buffer::~Buffer(void)
{
	Destroy();
}

bool Buffer::Create(const void *data, int size)
{
	SGE_ASSERT(m_id == 0);
	SGE_ASSERT(size > 0);

	glGenBuffers(1, &m_id);
	if (m_id == 0)
		return false;

	glBindBuffer(m_target, m_id);
	glBufferData(m_target, size, data, m_usage);

	m_size = size;

	return true;
}

void Buffer::Destroy(void)
{
	if (m_id > 0)
		glDeleteBuffers(1, &m_id);
}

SGE_GL_END
