//
//
#ifndef SGE_GL_SHADER_H
#define SGE_GL_SHADER_H

#include <string>

#include <sge/gl/common.h>

SGE_GL_BEGIN

class Shader {
public:
	Shader(GLenum type);
	virtual ~Shader(void);

public:
	bool Create(void);
	void Destroy(void);
	GLuint GetID(void) const;
	GLenum GetType(void) const;
	bool Compile(const char *src);
	const char *GetInfoLog(void) const;

private:
	GLuint m_id;
	GLenum m_type;
	bool m_compiled;
	std::string m_info_log;
};

inline GLuint Shader::GetID(void) const
{
	return m_id;
}

inline GLenum Shader::GetType(void) const
{
	return m_type;
}

inline const char *Shader::GetInfoLog(void) const
{
	return m_info_log.c_str();
}

SGE_GL_END

#endif // SGE_GL_SHADER_H
