//
//
#include <sge/gl/buffer.hpp>

SGE_GL_BEGIN

buffer::buffer(GLenum target, GLenum usage)
	: m_id(0)
	, m_target(target)
	, m_usage(usage)
	, m_mapped(false)
{
}

buffer::~buffer(void)
{
	destroy();
}

bool buffer::create(const void *data, int size)
{
	SGE_ASSERT(m_id == 0);
	SGE_ASSERT(size > 0);

	context::object::init();

	glGenBuffers(1, &m_id);
	if (m_id == 0)
		return false;

	glBindBuffer(m_target, m_id);
	glBufferData(m_target, size, data, m_usage);

	m_size = size;

	return true;
}

void buffer::destroy(void)
{
	if (m_id > 0)
		glDeleteBuffers(1, &m_id);

	context::object::shutdown();
}

SGE_GL_END

