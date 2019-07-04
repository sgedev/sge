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
	Destroy();
}

bool VertexArray::Create(void)
{
	GL_ASSERT(m_id == 0);

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

GL_END

