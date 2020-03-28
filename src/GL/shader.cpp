//
//
#include <GL/shader.hpp>

GL_BEGIN

Shader::Shader(void)
	: m_id(0)
{
}

Shader::~Shader(void)
{
	if (m_id > 0)
		glDeleteShader(m_id);
}

GL_END
