//
//
#include <GL/vertexarray.hpp>

GL_BEGIN

VertexArray::VertexArray(void)
	: m_id(0)
{
}

VertexArray::~VertexArray(void)
{
	if (m_id)
		glDeleteVertexArrays(1, &m_id);
}

GL_END
