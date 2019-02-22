//
//
#ifndef SGL_GL_PROGRAM_H
#define SGL_GL_PROGRAM_H

#include <string>
#include <Eigen/Geometry>

#include "SGL_gl_common.h"

SGL_GL_BEGIN

class Program {
public:
	Program(void);
	virtual ~Program(void);

public:
	bool Create(void);
	void Destroy(void);
	GLuint Id(void) const;
	bool AddShader(GLuint shader_id);
	bool AddShader(GLenum type, const char *src);
	bool Link(void);
	GLint GetUniformLocation(const char *name);
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
	void Uniform(GLint location, GLsizei count, const Eigen::Vector2f *value);
	void Uniform(GLint location, GLsizei count, const Eigen::Vector2i *value);
	void Uniform(GLint location, GLsizei count, const Eigen::Vector3f *value);
	void Uniform(GLint location, GLsizei count, const Eigen::Vector3i *value);
	void Uniform(GLint location, GLsizei count, const Eigen::Vector4f *value);
	void Uniform(GLint location, GLsizei count, const Eigen::Vector4i *value);
	void Uniform(GLint location, GLsizei count, GLboolean transpose, const Eigen::Matrix2f *value);
	void Uniform(GLint location, GLsizei count, GLboolean transpose, const Eigen::Matrix3f *value);
	void Uniform(GLint location, GLsizei count, GLboolean transpose, const Eigen::Matrix4f *value);

private:
	GLuint m_id;
	std::string m_info_log;
};

inline GLuint Program::Id(void) const
{
	return m_id;
}

inline bool Program::AddShader(GLuint shader_id)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(shader_id > 0);

	glAttachShader(m_id, shader_id);
}

inline GLint Program::GetUniformLocation(const char *name)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(name != NULL);

	return glGetUniformLocation(m_id, name);
}

inline void Program::Uniform(GLint location, GLfloat v0)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform1f(location, v0);
}

inline void Program::Uniform(GLint location, GLfloat v0, GLfloat v1)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform2f(location, v0, v1);
}

inline void Program::Uniform(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform3f(location, v0, v1, v2);
}

inline void Program::Uniform(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform4f(location, v0, v1, v2, v3);
}

inline void Program::Uniform(GLint location, GLint v0)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform1i(location, v0);
}

inline void Program::Uniform(GLint location, GLint v0, GLint v1)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform2i(location, v0, v1);
}

inline void Program::Uniform(GLint location, GLint v0, GLint v1, GLint v2)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform3i(location, v0, v1, v2);
}

inline void Program::Uniform(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform3i(location, v0, v2, v3);
}

inline void Program::Uniform(GLint location, GLsizei count, const GLfloat *value)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform1fv(location, count, value);
}

inline void Program::Uniform(GLint location, GLsizei count, const GLint *value)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform1iv(location, count, value);
}

inline void Program::Uniform(GLint location, GLsizei count, const Eigen::Vector2f *value)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform2fv(location, count, value->data());
}

inline void Program::Uniform(GLint location, GLsizei count, const Eigen::Vector2i *value)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform2iv(location, count, value->data());
}

inline void Program::Uniform(GLint location, GLsizei count, const Eigen::Vector3f *value)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform3fv(location, count, value->data());
}

inline void Program::Uniform(GLint location, GLsizei count, const Eigen::Vector3i *value)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform3iv(location, count, value->data());
}

inline void Program::Uniform(GLint location, GLsizei count, const Eigen::Vector4f *value)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform4fv(location, count, value->data());
}

inline void Program::Uniform(GLint location, GLsizei count, const Eigen::Vector4i *value)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniform4iv(location, count, value->data());
}

inline void Program::Uniform(GLint location, GLsizei count, GLboolean transpose, const Eigen::Matrix2f *value)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniformMatrix2fv(location, count, transpose, value->data());
}

inline void Program::Uniform(GLint location, GLsizei count, GLboolean transpose, const Eigen::Matrix3f *value)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniformMatrix3fv(location, count, transpose, value->data());
}

inline void Program::Uniform(GLint location, GLsizei count, GLboolean transpose, const Eigen::Matrix4f *value)
{
	SGL_ASSERT(m_id > 0);
	SGL_ASSERT(location >= 0);

	glUniformMatrix4fv(location, count, transpose, value->data());
}

SGL_GL_END

#endif // SGL_GL_PROGRAM_H

