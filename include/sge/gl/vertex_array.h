//
//
#ifndef SGE_GL_VERTEX_ARRAY_H
#define SGE_GL_VERTEX_ARRAY_H

#include <sge/gl/common.h>

SGE_GL_BEGIN

class VertexArray {
public:
	VertexArray(void);
	virtual ~VertexArray(void);

public:
	bool Create(void);
	void Destroy(void);
	GLuint GetID(void) const;
	void Bind(void);

private:
	GLuint m_id;
};

inline GLuint VertexArray::GetID(void) const
{
	return m_id;
}

inline void VertexArray::Bind(void)
{
	SGE_ASSERT(m_id > 0);

	glBindVertexArray(m_id);
}

SGE_GL_END

#endif // SGE_GL_VERTEX_ARRAY_H
