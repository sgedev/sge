//
//
#ifndef GL_SHADER_HPP
#define GL_SHADER_HPP

#include <string>

#include <GL/common.hpp>

GL_BEGIN

class Shader {
public:
	Shader(GLenum type);
	virtual ~Shader(void);

public:
	bool create(void);
	void destroy(void);
	GLuint id(void) const;
	GLenum type(void) const;
	bool compile(const char *src);
	const char *infoLog(void) const;

private:
	GLuint m_id;
	GLenum m_type;
	bool m_compiled;
	std::string m_infoLog;
};

inline GLuint Shader::id(void) const
{
	return m_id;
}

inline GLenum Shader::type(void) const
{
	return m_type;
}

inline const char *Shader::infoLog(void) const
{
	return m_infoLog.c_str();
}

GL_END

#endif // GL_SHADER_HPP

