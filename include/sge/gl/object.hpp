//
//
#ifndef SGE_GL_OBJECT_HPP
#define SGE_GL_OBJECT_HPP

#include <sge/gl/common.hpp>

SGE_GL_BEGIN

class object {
public:
	object(void);
	virtual ~object(void);

public:
	void enable(void);
	void disable(void);
	bool is_enabled(void) const;
	void set_transform(const glm::mat4 &v);
	const glm::mat4 &get_transform(void);

private:
	bool m_enabled;
	glm::mat4 m_transform;
};

inline void object::enable(void)
{
	m_enabled = true;
}

inline void object::disable(void)
{
	m_enabled = false;
}

inline bool object::is_enabled(void) const
{
	return m_enabled;
}

inline void object::set_transform(const glm::mat4 &v)
{
	m_transform = v;
}

inline const glm::mat4 &object::get_transform(void)
{
	return m_transform;
}

SGE_GL_END

#endif // SGE_GL_OBJECT_HPP

