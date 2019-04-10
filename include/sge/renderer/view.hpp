//
//
#ifndef SGE_RENDERER_VIEW_HPP
#define SGE_RENDERER_VIEW_HPP

#include <sge/noncopyable.hpp>
#include <sge/renderer/common.hpp>

SGE_RENDERER_BEGIN

class view final: public noncopyable {
	friend class context;

private:
	view(void);
	~view(void);

public:
	void enable(void);
	void disable(void);
	bool is_enabled(void) const;
	void enable_clear(void);
	void disable_clear(void);
	void set_viewport(int x, int y, int width, int height);
	void set_viewport(const glm::ivec4 &vp);
	void set_background(float red, float green, float blue);
	void set_background(const glm::vec3 &rgb);
	void set_model_view_matrix(const glm::mat4 &mat);
	void set_project_matrix(const glm::mat4 &mat);

private:
	bool init(void);
	void shutdown(void);
	void draw(void);

private:
	bool m_enabled;
	bool m_clear;
	glm::ivec4 m_viewport;
	glm::vec3 m_background;
	glm::mat4 m_model_view_matrix;
	glm::mat4 m_project_matrix;
	glm::mat4 m_normal_matrix;
};

inline void view::enable(void)
{
	m_enabled = true;
}

inline void view::disable(void)
{
	m_enabled = false;
}

inline bool view::is_enabled(void) const
{
	return m_enabled;
}

inline void view::enable_clear(void)
{
	m_clear = true;
}

inline void view::disable_clear(void)
{
	m_clear = false;
}

inline void view::set_viewport(int x, int y, int width, int height)
{
	SGE_ASSERT(width > 0);
	SGE_ASSERT(height > 0);

	m_viewport[0] = x;
	m_viewport[1] = y;
	m_viewport[2] = width;
	m_viewport[3] = height;
}

inline void view::set_viewport(const glm::ivec4 &vp)
{
	set_viewport(vp[0], vp[1], vp[2], vp[3]);
}

inline void view::set_background(float red, float green, float blue)
{
	m_background.r = red;
	m_background.g = green;
	m_background.b = blue;
}

inline void view::set_background(const glm::vec3 &rgb)
{
	set_background(rgb.r, rgb.g, rgb.b);
}

inline void view::set_model_view_matrix(const glm::mat4 &mat)
{
	m_model_view_matrix = mat;
}

inline void view::set_project_matrix(const glm::mat4 &mat)
{
	m_project_matrix = mat;
}

SGE_RENDERER_END

#endif // SGE_RENDERER_VIEW_HPP

