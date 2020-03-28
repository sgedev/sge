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
	GLuint id(void) const;

private:
	GLuint m_id;
};

inline GLuint VertexArray::id(void) const
{
	return m_id;
}

GL_END

#endif // GL_VERTEXARRAY_HPP
