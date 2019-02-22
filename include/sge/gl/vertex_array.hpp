//
//
#ifndef SGL_GL_VERTEX_ARRAY_H
#define SGL_GL_VERTEX_ARRAY_H

#include "SGL_gl_common.h"

SGL_GL_BEGIN

class VertexArray {
public:
	VertexArray(void);
	virtual ~VertexArray(void);

public:
	bool Create(void);
	void Destroy(void);
	GLuint Id(void) const;
	void Bind(void);

private:
	GLuint m_id;
};

inline GLuint VertexArray::Id(void) const
{
	return m_id;
}

inline void VertexArray::Bind(void)
{
	SGL_ASSERT(m_id > 0);
	glBindVertexArray(m_id);
}

SGL_GL_END

#endif // SGL_GL_VERTEX_ARRAY_H

