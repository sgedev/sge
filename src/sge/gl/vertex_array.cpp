//
//
#include <sge/gl/vertex_array.hpp>

SGE_GL_BEGIN

vertex_array::vertex_array(void)
	: m_id(0)
{
}

vertex_array::~vertex_array(void)
{
	if (m_id > 0)
		destroy();
}

bool vertex_array::create(void)
{
	SGE_ASSERT(m_id == 0);

	context::object::init();

	glGenVertexArrays(1, &m_id);
	if (m_id == 0)
		return false;

	return true;
}

void vertex_array::destroy(void)
{
	SGE_ASSERT(m_id > 0);

	glDeleteVertexArrays(1, &m_id);
	m_id = 0;
	
	context::object::shutdown();
}

SGE_GL_END

