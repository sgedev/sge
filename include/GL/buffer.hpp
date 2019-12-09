//
//
#ifndef GL_BUFFER_HPP
#define GL_BUFFER_HPP

#include <GL/common.hpp>

GL_BEGIN

class Buffer {
public:
	Buffer(GLenum target, GLenum usage);
	virtual ~Buffer(void);

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
	void *map(GLenum access, int offset, int size);
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

inline bool Buffer::create(int size)
{
	return create(NULL, size);
}

inline GLuint Buffer::id(void) const
{
	return m_id;
}

inline GLenum Buffer::target(void) const
{
	return m_target;
}

inline int Buffer::size(void) const
{
	return m_size;
}

inline void Buffer::bind(void)
{
	GL_ASSERT(m_id > 0);

	glBindBuffer(m_target, m_id);
}

inline void *Buffer::map(GLenum access)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(!m_mapped);

	void *p = glMapBuffer(m_target, access);
	if (p != NULL)
		m_mapped = true;

	return p;
}

inline void *Buffer::map(GLenum access, int offset, int size)
{
	/* TODO */
	return NULL;
}

inline bool Buffer::unmap(void)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(m_mapped);

	if (!glUnmapBuffer(m_target))
		return false;

	m_mapped = false;

	return true;
}

inline bool Buffer::read(int offset, void *buff, int size)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(offset >= 0);
	GL_ASSERT(offset < m_size);
	GL_ASSERT(buff != NULL);
	GL_ASSERT((offset + size) > 0);
	GL_ASSERT((offset + size) < m_size);
	GL_ASSERT(!m_mapped);

	glGetBufferSubData(m_target, offset, size, buff);

	return true;
}

inline bool Buffer::write(int offset, const void *data, int size)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(offset >= 0);
	GL_ASSERT(offset < m_size);
	GL_ASSERT(data != NULL);
	GL_ASSERT((offset + size) > 0);
	GL_ASSERT((offset + size) < m_size);
	GL_ASSERT(!m_mapped);

	glBufferSubData(m_target, offset, size, data);

	return true;
}

GL_END

#endif // GL_BUFFER_HPP

