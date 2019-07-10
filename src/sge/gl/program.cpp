//
//
#include <sge/gl/program.hpp>

SGE_GL_BEGIN

program::program(void)
	: m_id(0)
{
}

program::~program(void)
{
	destroy();
}

bool program::create(void)
{
	SGE_ASSERT(m_id == 0);

	m_id = glCreateProgram();
	if (m_id == 0)
		return false;

	return true;
}

void program::destroy(void)
{
	if (m_id != 0) {
		glDeleteProgram(m_id);
		m_id = 0;
	}
}

bool program::add_shader(GLenum type, const char *src)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(src != NULL);

	GLuint shader = glCreateShader(type);
	if (shader == 0)
		return false;

	int len[1];
	len[0] = (int)strlen(src);
	SGE_ASSERT(len[0] > 0);

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

	return false;
}

bool program::link(void)
{
	SGE_ASSERT(m_id > 0);

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

SGE_GL_END

