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
	bool Create(const void *data, int size);
	bool Create(int size);
	void Destroy(void);
	GLuint Id(void) const;
	GLenum Type(void) const;
	GLenum Target(void) const;
	int Size(void) const;
	void Bind(void);
	void *Map(GLenum access);
	void *Map(GLenum access, int offset, int size);
	bool Unmap(void);
	bool Read(int offset, void *buff, int size);
	bool Write(int offset, const void *data, int size);

private:
	GLuint m_id;
	GLenum m_target;
	GLenum m_usage;
	GLint m_size;
	bool m_mapped;
};

inline bool Buffer::Create(int size)
{
	return Create(NULL, size);
}

inline GLuint Buffer::Id(void) const
{
	return m_id;
}

inline GLenum Buffer::Target(void) const
{
	return m_target;
}

inline int Buffer::Size(void) const
{
	return m_size;
}

inline void Buffer::Bind(void)
{
	GL_ASSERT(m_id > 0);

	glBindBuffer(m_target, m_id);
}

inline void *Buffer::Map(GLenum access)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(!m_mapped);

	void *p = glMapBuffer(m_target, access);
	if (p != NULL)
		m_mapped = true;

	return p;
}

inline void *Buffer::Map(GLenum access, int offset, int size)
{
	/* TODO */
	return NULL;
}

inline bool Buffer::Unmap(void)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(m_mapped);

	if (!glUnmapBuffer(m_target))
		return false;

	m_mapped = false;

	return true;
}

inline bool Buffer::Read(int offset, void *buff, int size)
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

inline bool Buffer::Write(int offset, const void *data, int size)
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

