//
//
#include <string.h>

#include <sge/gl/shader.h>

SGE_GL_BEGIN

shader::shader(GLenum type)
	: m_id(0)
	, m_type(type)
{
}

shader::~shader(void)
{
	destroy();
}

bool shader::create(void)
{
	SGE_ASSERT(m_id == 0);

	m_id = glCreateShader(m_type);
	if (m_id == 0)
		return false;

	return true;
}

void shader::destroy(void)
{
	if (m_id > 0) {
		glDeleteShader(m_id);
		m_id = 0;
	}

	m_info_log.clear();
}

bool shader::compile(const char *src)
{
	SGE_ASSERT(m_id != 0);
	SGE_ASSERT(src != NULL);

	int len[1];
	len[0] = (int)strlen(src);
	SGE_ASSERT(len[0] > 0);

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

SGE_GL_END
