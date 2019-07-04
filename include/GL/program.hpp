//
//
#ifndef GL_PROGRAM_HPP
#define GL_PROGRAM_HPP

#include <string>

#include <GL/common.hpp>

GL_BEGIN

class Program {
public:
	Program(void);
	virtual ~Program(void);

public:
	bool Create(void);
	void Destroy(void);
	GLuint Id(void) const;
	const char *InfoLog(void) const;
	bool AddShader(GLuint shader_id);
	bool AddShader(GLenum type, const char *src);
	bool Link(void);
	void Use(void);
	GLint UniformLocation(const char *name);
	void Uniform(GLint location, GLfloat v0);
	void Uniform(GLint location, GLfloat v0, GLfloat v1);
	void Uniform(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	void Uniform(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	void Uniform(GLint location, GLint v0);
	void Uniform(GLint location, GLint v0, GLint v1);
	void Uniform(GLint location, GLint v0, GLint v1, GLint v2);
	void Uniform(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
	void Uniform(GLint location, GLsizei count, const GLfloat *value);
	void Uniform(GLint location, GLsizei count, const GLint *value);
	void Uniform(GLint location, GLsizei count, const glm::vec2 *value);
	void Uniform(GLint location, GLsizei count, const glm::ivec2 *value);
	void Uniform(GLint location, GLsizei count, const glm::vec3 *value);
	void Uniform(GLint location, GLsizei count, const glm::ivec3 *value);
	void Uniform(GLint location, GLsizei count, const glm::vec4 *value);
	void Uniform(GLint location, GLsizei count, const glm::ivec4 *value);
	void Uniform(GLint location, GLsizei count, GLboolean transpose, const glm::mat2 *value);
	void Uniform(GLint location, GLsizei count, GLboolean transpose, const glm::mat3 *value);
	void Uniform(GLint location, GLsizei count, GLboolean transpose, const glm::mat4 *value);

private:
	GLuint m_id;
	std::string m_info_log;
};

inline GLuint Program::Id(void) const
{
	return m_id;
}

inline const char *Program::InfoLog(void) const
{
	return m_info_log.c_str();
}
inline bool Program::AddShader(GLuint shader_id)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(shader_id > 0);

	glAttachShader(m_id, shader_id);
}

inline void Program::Use(void)
{
	GL_ASSERT(m_id > 0);

	glUseProgram(m_id);
}

inline GLint Program::UniformLocation(const char *name)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(name != NULL);

	return glGetUniformLocation(m_id, name);
}

inline void Program::Uniform(GLint location, GLfloat v0)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform1f(location, v0);
}

inline void Program::Uniform(GLint location, GLfloat v0, GLfloat v1)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform2f(location, v0, v1);
}

inline void Program::Uniform(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform3f(location, v0, v1, v2);
}

inline void Program::Uniform(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform4f(location, v0, v1, v2, v3);
}

inline void Program::Uniform(GLint location, GLint v0)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform1i(location, v0);
}

inline void Program::Uniform(GLint location, GLint v0, GLint v1)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform2i(location, v0, v1);
}

inline void Program::Uniform(GLint location, GLint v0, GLint v1, GLint v2)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform3i(location, v0, v1, v2);
}

inline void Program::Uniform(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform3i(location, v0, v2, v3);
}

inline void Program::Uniform(GLint location, GLsizei count, const GLfloat *value)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform1fv(location, count, value);
}

inline void Program::Uniform(GLint location, GLsizei count, const GLint *value)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform1iv(location, count, value);
}

inline void Program::Uniform(GLint location, GLsizei count, const glm::vec2 *value)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform2fv(location, count, glm::value_ptr(*value));
}

inline void Program::Uniform(GLint location, GLsizei count, const glm::ivec2 *value)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform2iv(location, count, glm::value_ptr(*value));
}

inline void Program::Uniform(GLint location, GLsizei count, const glm::vec3 *value)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform3fv(location, count, glm::value_ptr(*value));
}

inline void Program::Uniform(GLint location, GLsizei count, const glm::ivec3 *value)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform3iv(location, count, glm::value_ptr(*value));
}

inline void Program::Uniform(GLint location, GLsizei count, const glm::vec4 *value)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform4fv(location, count, glm::value_ptr(*value));
}

inline void Program::Uniform(GLint location, GLsizei count, const glm::ivec4 *value)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniform4iv(location, count, glm::value_ptr(*value));
}

inline void Program::Uniform(GLint location, GLsizei count, GLboolean transpose, const glm::mat2 *value)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniformMatrix2fv(location, count, transpose, glm::value_ptr(*value));
}

inline void Program::Uniform(GLint location, GLsizei count, GLboolean transpose, const glm::mat3 *value)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniformMatrix3fv(location, count, transpose, glm::value_ptr(*value));
}

inline void Program::Uniform(GLint location, GLsizei count, GLboolean transpose, const glm::mat4 *value)
{
	GL_ASSERT(m_id > 0);
	GL_ASSERT(location >= 0);

	glUniformMatrix4fv(location, count, transpose, glm::value_ptr(*value));
}

GL_END

#endif // GL_PROGRAM_HPP

