//
//
#ifndef GL_VERTEXARRAY_HPP
#define GL_VERTEXARRAY_HPP

#include <GL/common.hpp>

GL_BEGIN

class VertexArray {
public:
	VertexArray(void);
	virtual ~VertexArray(void);

public:
	bool Create(void);
	void Destroy(void);
	GLuint Id(void) const;
	void Bind(void);

private:
	GLuint m_id;
};

inline GLuint VertexArray::Id(void) const
{
	return m_id;
}

inline void VertexArray::Bind(void)
{
	GL_ASSERT(m_id > 0);

	glBindVertexArray(m_id);
}

GL_END

#endif // GL_VERTEXARRAY_HPP

