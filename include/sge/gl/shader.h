//
//
#ifndef SGE_GL_SHADER_H
#define SGE_GL_SHADER_H

#include <string>

#include <sge/gl/common.h>

SGE_GL_BEGIN

class shader {
public:
	shader(GLenum type);
	virtual ~shader(void);

public:
	bool create(void);
	void destroy(void);
	GLuint id(void) const;
	GLenum type(void) const;
	bool compile(const char *src);
	const char *info_log(void) const;

private:
	GLuint m_id;
	GLenum m_type;
	bool m_compiled;
	std::string m_info_log;
};

inline GLuint shader::id(void) const
{
	return m_id;
}

inline GLenum shader::type(void) const
{
	return m_type;
}

inline const char *shader::info_log(void) const
{
	return m_info_log.c_str();
}

SGE_GL_END

#endif // SGE_GL_SHADER_H
