//
//
#ifndef GL_VERTEXARRAY_HPP
#define GL_VERTEXARRAY_HPP

#include <GL/common.hpp>

GL_BEGIN

class VertexArray {
public:
	VertexArray(void);
	virtual ~VertexArray(void);

public:
	bool create(void);
	void destroy(void);
	GLuint id(void) const;
	void bind(void);

private:
	GLuint m_id;
};

inline GLuint VertexArray::id(void) const
{
	return m_id;
}

inline void VertexArray::bind(void)
{
	GL_ASSERT(m_id > 0);

	glBindVertexArray(m_id);
}

GL_END

#endif // GL_VERTEX_ARRAY_HPP

