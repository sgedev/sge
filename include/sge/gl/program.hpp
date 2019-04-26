//
//
#ifndef SGE_GL_PROGRAM_HPP
#define SGE_GL_PROGRAM_HPP

#include <string>

#include <sge/gl/common.hpp>

SGE_GL_BEGIN

class program {
public:
	program(void);
	virtual ~program(void);

public:
	bool create(void);
	void destroy(void);
	GLuint id(void) const;
	const char *info_log(void) const;
	bool add_shader(GLuint shader_id);
	bool add_shader(GLenum type, const char *src);
	bool link(void);
	void use(void);
	GLint uniform_location(const char *name);
	void uniform(GLint location, GLfloat v0);
	void uniform(GLint location, GLfloat v0, GLfloat v1);
	void uniform(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	void uniform(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	void uniform(GLint location, GLint v0);
	void uniform(GLint location, GLint v0, GLint v1);
	void uniform(GLint location, GLint v0, GLint v1, GLint v2);
	void uniform(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
	void uniform(GLint location, GLsizei count, const GLfloat *value);
	void uniform(GLint location, GLsizei count, const GLint *value);
	void uniform(GLint location, GLsizei count, const glm::vec2 *value);
	void uniform(GLint location, GLsizei count, const glm::ivec2 *value);
	void uniform(GLint location, GLsizei count, const glm::vec3 *value);
	void uniform(GLint location, GLsizei count, const glm::ivec3 *value);
	void uniform(GLint location, GLsizei count, const glm::vec4 *value);
	void uniform(GLint location, GLsizei count, const glm::ivec4 *value);
	void uniform(GLint location, GLsizei count, GLboolean transpose, const glm::mat2 *value);
	void uniform(GLint location, GLsizei count, GLboolean transpose, const glm::mat3 *value);
	void uniform(GLint location, GLsizei count, GLboolean transpose, const glm::mat4 *value);

private:
	GLuint m_id;
	std::string m_info_log;
};

inline GLuint program::id(void) const
{
	return m_id;
}

inline const char *program::info_log(void) const
{
	return m_info_log.c_str();
}

inline bool program::add_shader(GLuint shader_id)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(shader_id > 0);

	glAttachShader(m_id, shader_id);
}

inline void program::use(void)
{
	SGE_ASSERT(m_id > 0);

	glUseProgram(m_id);
}

inline GLint program::uniform_location(const char *name)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(name != NULL);

	return glGetUniformLocation(m_id, name);
}

inline void program::uniform(GLint location, GLfloat v0)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform1f(location, v0);
}

inline void program::uniform(GLint location, GLfloat v0, GLfloat v1)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform2f(location, v0, v1);
}

inline void program::uniform(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform3f(location, v0, v1, v2);
}

inline void program::uniform(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform4f(location, v0, v1, v2, v3);
}

inline void program::uniform(GLint location, GLint v0)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform1i(location, v0);
}

inline void program::uniform(GLint location, GLint v0, GLint v1)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform2i(location, v0, v1);
}

inline void program::uniform(GLint location, GLint v0, GLint v1, GLint v2)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform3i(location, v0, v1, v2);
}

inline void program::uniform(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform3i(location, v0, v2, v3);
}

inline void program::uniform(GLint location, GLsizei count, const GLfloat *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform1fv(location, count, value);
}

inline void program::uniform(GLint location, GLsizei count, const GLint *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform1iv(location, count, value);
}

inline void program::uniform(GLint location, GLsizei count, const glm::vec2 *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform2fv(location, count, glm::value_ptr(*value));
}

inline void program::uniform(GLint location, GLsizei count, const glm::ivec2 *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform2iv(location, count, glm::value_ptr(*value));
}

inline void program::uniform(GLint location, GLsizei count, const glm::vec3 *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform3fv(location, count, glm::value_ptr(*value));
}

inline void program::uniform(GLint location, GLsizei count, const glm::ivec3 *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform3iv(location, count, glm::value_ptr(*value));
}

inline void program::uniform(GLint location, GLsizei count, const glm::vec4 *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform4fv(location, count, glm::value_ptr(*value));
}

inline void program::uniform(GLint location, GLsizei count, const glm::ivec4 *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform4iv(location, count, glm::value_ptr(*value));
}

inline void program::uniform(GLint location, GLsizei count, GLboolean transpose, const glm::mat2 *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniformMatrix2fv(location, count, transpose, glm::value_ptr(*value));
}

inline void program::uniform(GLint location, GLsizei count, GLboolean transpose, const glm::mat3 *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniformMatrix3fv(location, count, transpose, glm::value_ptr(*value));
}

inline void program::uniform(GLint location, GLsizei count, GLboolean transpose, const glm::mat4 *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniformMatrix4fv(location, count, transpose, glm::value_ptr(*value));
}

SGE_GL_END

#endif // SGE_GL_PROGRAM_HPP

