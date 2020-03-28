//
//
#ifndef GL_PROGRAM_HPP
#define GL_PROGRAM_HPP

#include <GL/common.hpp>

GL_BEGIN

class Program {
public:
	Program(void);
	virtual ~Program(void);

public:
	GLuint id(void) const;

private:
	GLuint m_id;
};

inline GLuint Program::id(void) const
{
	return m_id;
}

GL_END

#endif // GL_PROGRAM_HPP
