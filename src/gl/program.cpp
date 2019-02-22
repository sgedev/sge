//
//
#include "SGL_gl_program.h"

SGL_GL_BEGIN

Program::Program(void)
	: m_id(0)
{
}

Program::~Program(void)
{
	Destroy();
}

bool Program::Create(void)
{
	SGL_ASSERT(m_id == 0);

	m_id = glCreateProgram();
	if (m_id == 0)
		return false;

	return true;
}

void Program::Destroy(void)
{
	if (m_id != 0) {
		glDeleteProgram(m_id);
		m_id = 0;
	}
}

bool Program::AddShader(GLenum type, const char *src)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(src != NULL);

	GLuint shader = glCreateShader(type);
	if (shader == 0)
		return false;

	int len[1];
	len[0] = (int)strlen(src);
	SGL_ASSERT(len[0] > 0);

	glShaderSource(shader, 1, &src, len);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE) {
		glAttachShader(m_id, shader);
		return true;
	}

	GLsizei length;
	GLchar info_log[256] = { 0 };
	glGetShaderInfoLog(shader, sizeof(info_log) - 1, &length, info_log);
	m_info_log = info_log;

	return true;
}

bool Program::Link(void)
{
	SGL_ASSERT(m_id > 0);

	glLinkProgram(m_id);

	GLint status;
	glGetProgramiv(m_id, GL_LINK_STATUS, &status);
	if (status == GL_TRUE)
		return true;

	GLsizei length;
	GLchar info_log[256] = { 0 };
	glGetProgramInfoLog(m_id, sizeof(info_log) - 1, &length, info_log);
	m_info_log = info_log;

	return false;
}

SGL_GL_END
