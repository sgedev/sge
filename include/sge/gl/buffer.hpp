//
//
#ifndef SGE_GL_BUFFER_HPP
#define SGE_GL_BUFFER_HPP

#include <sge/gl/common.hpp>

SGE_GL_BEGIN

class buffer {
public:
	buffer(GLenum target, GLenum usage);
	virtual ~buffer(void);

public:
	bool create(const void *data, int size);
	bool create(int size);
	void destroy(void);
	GLuint id(void) const;
	GLenum type(void) const;
	GLenum target(void) const;
	int size(void) const;
	void bind(void);
	void *map(GLenum access);
	void *map_range(int offset, int size, GLenum access);
	bool unmap(void);
	bool read(int offset, void *buff, int size);
	bool write(int offset, const void *data, int size);

private:
	GLuint m_id;
	GLenum m_target;
	GLenum m_usage;
	GLint m_size;
	bool m_mapped;
};

inline bool buffer::create(int size)
{
	return create(NULL, size);
}

inline GLuint buffer::id(void) const
{
	return m_id;
}

inline GLenum buffer::target(void) const
{
	return m_target;
}

inline int buffer::size(void) const
{
	return m_size;
}

inline void buffer::bind(void)
{
	SGE_ASSERT(m_id > 0);

	glBindBuffer(m_target, m_id);
}

inline void *buffer::map(GLenum access)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(!m_mapped);

	void *p = glMapBuffer(m_target, access);
	if (p != NULL)
		m_mapped = true;

	return p;
}

inline bool buffer::unmap(void)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(m_mapped);

	if (!glUnmapBuffer(m_target))
		return false;

	m_mapped = false;

	return true;
}

inline bool buffer::read(int offset, void *buff, int size)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(offset >= 0);
	SGE_ASSERT(offset < m_size);
	SGE_ASSERT(buff != NULL);
	SGE_ASSERT((offset + size) > 0);
	SGE_ASSERT((offset + size) < m_size);
	SGE_ASSERT(!m_mapped);

	glGetBufferSubData(m_target, offset, size, buff);

	return true;
}

inline bool buffer::write(int offset, const void *data, int size)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(offset >= 0);
	SGE_ASSERT(offset < m_size);
	SGE_ASSERT(data != NULL);
	SGE_ASSERT((offset + size) > 0);
	SGE_ASSERT((offset + size) < m_size);
	SGE_ASSERT(!m_mapped);

	glBufferSubData(m_target, offset, size, data);

	return true;
}

SGE_GL_END

#endif // SGE_GL_BUFFER_HPP
