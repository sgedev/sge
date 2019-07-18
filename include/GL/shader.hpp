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
	bool Create(void);
	void Destroy(void);
	GLuint Id(void) const;
	GLenum Type(void) const;
	bool Compile(const char *src);
	const char *InfoLog(void) const;

private:
	GLuint m_id;
	GLenum m_type;
	bool m_compiled;
	std::string m_info_log;
};

inline GLuint Shader::Id(void) const
{
	return m_id;
}

inline GLenum Shader::Type(void) const
{
	return m_type;
}

inline const char *Shader::InfoLog(void) const
{
	return m_info_log.c_str();
}

GL_END

#endif // GL_SHADER_HPP

