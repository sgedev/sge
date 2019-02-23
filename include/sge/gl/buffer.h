//
//
#ifndef SGE_GL_BUFFER_H
#define SGE_GL_BUFFER_H

#include <sge/gl/common.h>

SGE_GL_BEGIN

class Buffer {
public:
	Buffer(GLenum target, GLenum usage);
	virtual ~Buffer(void);

public:
	bool Create(const void *data, int size);
	bool Create(int size);
	void Destroy(void);
	GLuint GetID(void) const;
	GLenum GetType(void) const;
	GLenum GetTarget(void) const;
	int GetSize(void) const;
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

inline GLuint Buffer::GetID(void) const
{
	return m_id;
}

inline GLenum Buffer::GetTarget(void) const
{
	return m_target;
}

inline int Buffer::GetSize(void) const
{
	return m_size;
}

inline void Buffer::Bind(void)
{
	SGE_ASSERT(m_id > 0);

	glBindBuffer(m_target, m_id);
}

inline void *Buffer::Map(GLenum access)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(!m_mapped);

	void *p = glMapBuffer(m_target, access);
	if (p != NULL)
		m_mapped = true;

	return p;
}

inline bool Buffer::Unmap(void)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(m_mapped);

	if (!glUnmapBuffer(m_target))
		return false;

	m_mapped = false;

	return true;
}

inline bool Buffer::Read(int offset, void *buff, int size)
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

inline bool Buffer::Write(int offset, const void *data, int size)
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

#endif // SGE_GL_BUFFER_H
