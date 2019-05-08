//
//
#include <string.h>

#include <GL/shader.hpp>

GL_BEGIN

Shader::Shader(GLenum type)
	: m_id(0)
	, m_type(type)
{
}

Shader::~Shader(void)
{
	Destroy();
}

bool Shader::Create(void)
{
	GL_ASSERT(m_id == 0);

	m_id = glCreateShader(m_type);
	if (m_id == 0)
		return false;

	return true;
}

void Shader::Destroy(void)
{
	if (m_id > 0) {
		glDeleteShader(m_id);
		m_id = 0;
	}

	m_info_log.clear();
}

bool Shader::Compile(const char *src)
{
	GL_ASSERT(m_id != 0);
	GL_ASSERT(src != NULL);

	int len[1];
	len[0] = (int)strlen(src);
	GL_ASSERT(len[0] > 0);

	glShaderSource(m_id, 1, &src, len);

	glCompileShader(m_id);

	GLint status;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE)
		return true;

	GLsizei length;
	GLchar info_log[256] = { 0 };
	glGetShaderInfoLog(m_id, sizeof(info_log) - 1, &length, info_log);
	m_info_log = info_log;

	return false;
}

GL_END

