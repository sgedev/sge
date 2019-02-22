//
//
#ifndef SGL_GL_BUFFER_H
#define SGL_GL_BUFFER_H

#include "SGL_gl_common.h"

SGL_GL_BEGIN

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
	void *MapRange(int offset, int size, GLenum access);
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
	SGL_ASSERT(m_id > 0);

	glBindBuffer(m_target, m_id);
}

inline void *Buffer::Map(GLenum access)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(!m_mapped);

	void *p = glMapBuffer(m_target, access);
	if (p != NULL)
		m_mapped = true;

	return p;
}

inline bool Buffer::Unmap(void)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(m_mapped);

	if (!glUnmapBuffer(m_target))
		return false;

	m_mapped = false;

	return true;
}

inline bool Buffer::Read(int offset, void *buff, int size)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(offset >= 0);
	SGL_ASSERT(offset < m_size);
	SGL_ASSERT(buff != NULL);
	SGL_ASSERT((offset + size) > 0);
	SGL_ASSERT((offset + size) < m_size);
	SGL_ASSERT(!m_mapped);

	glGetBufferSubData(m_target, offset, size, buff);

	return true;
}

inline bool Buffer::Write(int offset, const void *data, int size)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(offset >= 0);
	SGL_ASSERT(offset < m_size);
	SGL_ASSERT(data != NULL);
	SGL_ASSERT((offset + size) > 0);
	SGL_ASSERT((offset + size) < m_size);
	SGL_ASSERT(!m_mapped);

	glBufferSubData(m_target, offset, size, data);

	return true;
}

SGL_GL_END

#endif // SGL_GL_BUFFER_H

