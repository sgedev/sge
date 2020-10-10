//
//
#ifndef SGE_GRAPHICS_WINDOW_HPP
#define SGE_GRAPHICS_WINDOW_HPP

#include <sge/graphics/common.hpp>
#include <sge/graphics/canvas.hpp>

SGE_GRAPHICS_BEGIN

class window: public canvas {
public:
	window(void);
	~window(void) override;

public:
	bool init(const char *name, int x, int y, int width, int height);
	void handle_event(const SDL_WindowEvent &evt);
	Uint32 id(void) const;
	bool visible(void) const;
	void show(void);
	void hide(void);
	const glm::ivec2 &position(void) const;
	void move(int x, int y);
	void move(const glm::ivec2 &r);
	void resize(int width, int height) override;

private:
	SDL_Window *m_handle;
	Uint32 m_id;
	glm::ivec2 m_position;
	bool m_visible;
	SDL_GLContext m_gl_context;
};

SGE_INLINE Uint32 window::id(void) const
{
	return m_id;
}

SGE_INLINE bool window::visible(void) const
{
	return m_visible;
}

SGE_INLINE void window::show(void)
{
	SGE_ASSERT(m_handle != nullptr);
	SDL_ShowWindow(m_handle);
}

SGE_INLINE void window::hide(void)
{
	SGE_ASSERT(m_handle != nullptr);
	SDL_HideWindow(m_handle);
}

SGE_INLINE const glm::ivec2 &window::position(void) const
{
	return m_position;
}

SGE_INLINE void window::move(int x, int y)
{
	SGE_ASSERT(m_handle != nullptr);
	SDL_SetWindowPosition(m_handle, x, y);
	SDL_GetWindowPosition(m_handle, &m_position.x, &m_position.y);
}

SGE_INLINE void window::move(const glm::ivec2 &r)
{
	move(r.x, r.y);
}

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_WINDOW_HPP
