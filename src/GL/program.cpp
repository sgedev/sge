//
//
#include <GL/program.hpp>

GL_BEGIN

Program::Program(void)
	: m_id(0)
{
}

Program::~Program(void)
{
	if (m_id > 0)
		glDeleteProgram(m_id);
}

GL_END
