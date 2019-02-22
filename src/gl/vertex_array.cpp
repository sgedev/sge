//
//
#include "SGL_gl_vertex_array.h"

SGL_GL_BEGIN

VertexArray::VertexArray(void)
	: m_id(0)
{
}

VertexArray::~VertexArray(void)
{
	Destroy();
}

bool VertexArray::Create(void)
{
	SGL_ASSERT(m_id == 0);

	glGenVertexArrays(1, &m_id);
	if (m_id == 0)
		return false;

	return true;
}

void VertexArray::Destroy(void)
{
	if (m_id > 0) {
		glDeleteVertexArrays(1, &m_id);
		m_id = 0;
	}
}

SGL_GL_END
