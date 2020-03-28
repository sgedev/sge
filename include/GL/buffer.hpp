//
//
#ifndef GL_BUFFER_HPP
#define GL_BUFFER_HPP

#include <GL/common.hpp>

GL_BEGIN

class Buffer {
public:
	Buffer(void);
	virtual ~Buffer(void);

public:
	GLuint id(void) const;

private:
	GLuint m_id;
};

inline GLuint Buffer::id(void) const
{
	return m_id;
}

GL_END

#endif // GL_BUFFER_HPP
