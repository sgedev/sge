//
//
#ifndef SGE_GL_VIEW_HPP
#define SGE_GL_VIEW_HPP

#include <sge/gl/common.hpp>
#include <sge/gl/context.hpp>

SGE_GL_BEGIN

class view {
public:
	view(void);
	virtual ~view(void);

public:
	bool init(void);
	void shutdown(void);
	void update(void);
	void enable(void);
	void disable(void);

public:
	void set_viewport(int x, int y, int width, int height);
	void set_viewport(const glm::ivec4 &v);
	void enable_clear(void);
	void disable_clear(void);
	void set_clear_color(float red, float green, float blue);
	void set_clear_color(const glm::vec3 &v);
	void set_transform(const glm::mat4 &v);
	void set_projection(const glm::mat4 &v);

private:
	context *m_owner;
	bool m_enabled;
	glm::ivec4 m_viewport;
	bool m_clear_enabled;
	glm::vec3 m_clear_color;
	glm::mat4 m_transform;
	glm::mat4 m_projection;
};

inline void view::enable(void)
{
	m_enabled = true;
}

inline void view::disable(void)
{
	m_enabled = false;
}

inline void view::set_viewport(int x, int y, int width, int height)
{
	m_viewport[0] = x;
	m_viewport[1] = y;
	m_viewport[2] = width;
	m_viewport[3] = height;
}

inline void view::set_viewport(const glm::ivec4 &v)
{
	m_viewport = v;
}

inline void view::enable_clear(void)
{
	m_clear_enabled = true;
}

inline void view::disable_clear(void)
{
	m_clear_enabled = false;
}

inline void view::set_clear_color(float red, float green, float blue)
{
	m_clear_color.r = red;
	m_clear_color.g = green;
	m_clear_color.b = blue;
}

inline void view::set_clear_color(const glm::vec3 &v)
{
	m_clear_color = v;
}

inline void view::set_transform(const glm::mat4 &v)
{
	m_transform = v;
}

inline void view::set_projection(const glm::mat4 &v)
{
	m_projection = v;
}

SGE_GL_END

#endif // SGE_GL_VIEW_HPP

