//
//
#ifndef GL_SHADER_HPP
#define GL_SHADER_HPP

#include <GL/common.hpp>

GL_BEGIN

class Shader {
public:
	Shader(void);
	virtual ~Shader(void);

public:
	GLuint id(void) const;

private:
	GLuint m_id;
};

inline GLuint Shader::id(void) const
{
	return m_id;
}

GL_END

#endif // GL_SHADER_HPP
