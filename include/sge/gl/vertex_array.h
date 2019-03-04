//
//
#ifndef SGE_GL_VERTEX_ARRAY_H
#define SGE_GL_VERTEX_ARRAY_H

#include <sge/gl/common.h>

SGE_GL_BEGIN

class vertex_array {
public:
	vertex_array(void);
	virtual ~vertex_array(void);

public:
	bool create(void);
	void destroy(void);
	GLuint id(void) const;
	void bind(void);

private:
	GLuint m_id;
};

inline GLuint vertex_array::id(void) const
{
	return m_id;
}

inline void vertex_array::bind(void)
{
	SGE_ASSERT(m_id > 0);

	glBindVertexArray(m_id);
}

SGE_GL_END

#endif // SGE_GL_VERTEX_ARRAY_H
