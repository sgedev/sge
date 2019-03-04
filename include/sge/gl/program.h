//
//
#ifndef SGE_GL_PROGRAM_H
#define SGE_GL_PROGRAM_H

#include <string>

#include <sge/gl/common.h>

SGE_GL_BEGIN

class program {
public:
	program(void);
	virtual ~program(void);

public:
	bool create(void);
	void destroy(void);
	GLuint id(void) const;
	bool add_shader(GLuint shader_id);
	bool add_shader(GLenum type, const char *src);
	bool link(void);
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
	void uniform(GLint location, GLsizei count, const Eigen::Vector2f *value);
	void uniform(GLint location, GLsizei count, const Eigen::Vector2i *value);
	void uniform(GLint location, GLsizei count, const Eigen::Vector3f *value);
	void uniform(GLint location, GLsizei count, const Eigen::Vector3i *value);
	void uniform(GLint location, GLsizei count, const Eigen::Vector4f *value);
	void uniform(GLint location, GLsizei count, const Eigen::Vector4i *value);
	void uniform(GLint location, GLsizei count, GLboolean transpose, const Eigen::Matrix2f *value);
	void uniform(GLint location, GLsizei count, GLboolean transpose, const Eigen::Matrix3f *value);
	void uniform(GLint location, GLsizei count, GLboolean transpose, const Eigen::Matrix4f *value);

private:
	GLuint m_id;
	std::string m_info_log;
};

inline GLuint program::id(void) const
{
	return m_id;
}

inline bool program::add_shader(GLuint shader_id)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(shader_id > 0);

	glAttachShader(m_id, shader_id);
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

inline void program::uniform(GLint location, GLsizei count, const Eigen::Vector2f *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform2fv(location, count, value->data());
}

inline void program::uniform(GLint location, GLsizei count, const Eigen::Vector2i *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform2iv(location, count, value->data());
}

inline void program::uniform(GLint location, GLsizei count, const Eigen::Vector3f *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform3fv(location, count, value->data());
}

inline void program::uniform(GLint location, GLsizei count, const Eigen::Vector3i *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform3iv(location, count, value->data());
}

inline void program::uniform(GLint location, GLsizei count, const Eigen::Vector4f *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform4fv(location, count, value->data());
}

inline void program::uniform(GLint location, GLsizei count, const Eigen::Vector4i *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniform4iv(location, count, value->data());
}

inline void program::uniform(GLint location, GLsizei count, GLboolean transpose, const Eigen::Matrix2f *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniformMatrix2fv(location, count, transpose, value->data());
}

inline void program::uniform(GLint location, GLsizei count, GLboolean transpose, const Eigen::Matrix3f *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniformMatrix3fv(location, count, transpose, value->data());
}

inline void program::uniform(GLint location, GLsizei count, GLboolean transpose, const Eigen::Matrix4f *value)
{
	SGE_ASSERT(m_id > 0);
	SGE_ASSERT(location >= 0);

	glUniformMatrix4fv(location, count, transpose, value->data());
}

SGE_GL_END

#endif // SGE_GL_PROGRAM_H
