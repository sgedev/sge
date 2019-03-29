//
//
#ifndef SGE_GL_VERTEX_ARRAY_HPP
#define SGE_GL_VERTEX_ARRAY_HPP

#include <sge/gl/common.hpp>

SGE_GL_BEGIN

class vertex_array {
public:
	vertex_array(void);
	virtual ~vertex_array(void);

public:
	bool create(void);
	void destroy(void);
	GLuint id(void) const;

private:
	GLuint m_id;
};

inline GLuint vertex_array::id(void) const
{
	return m_id;
}

SGE_GL_END

#endif // SGE_GL_VERTEX_ARRAY_HPP

