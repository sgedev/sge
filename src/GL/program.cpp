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
	destroy();
}

bool Program::create(void)
{
	GL_ASSERT(m_id == 0);

	m_id = glCreateProgram();
	if (m_id == 0)
		return false;

	return true;
}

void Program::destroy(void)
{
	if (m_id != 0) {
		glDeleteProgram(m_id);
		m_id = 0;
	}
}

bool Program::addShader(GLenum type, const char *src)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(src != NULL);

	GLuint shader = glCreateShader(type);
	if (shader == 0)
		return false;

	int len[1];
	len[0] = (int)strlen(src);
	GL_ASSERT(len[0] > 0);

	glShaderSource(shader, 1, &src, len);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE) {
		glAttachShader(m_id, shader);
		return true;
	}

	GLsizei length;
	GLchar infoLog[256] = { 0 };
	glGetShaderInfoLog(shader, sizeof(infoLog) - 1, &length, infoLog);
	m_infoLog = infoLog;

	return false;
}

bool Program::link(void)
{
	GL_ASSERT(m_id > 0);

	glLinkProgram(m_id);

	GLint status;
	glGetProgramiv(m_id, GL_LINK_STATUS, &status);
	if (status == GL_TRUE)
		return true;

	GLsizei length;
	GLchar infoLog[256] = { 0 };
	glGetProgramInfoLog(m_id, sizeof(infoLog) - 1, &length, infoLog);
	m_infoLog = infoLog;

	return false;
}

GL_END

