//
//
#ifndef SGL_WINDOW_HPP
#define SGL_WINDOW_HPP

#include <SGL/Common.hpp>

SGL_BEGIN

class Window {
public:
	Window(void);
	virtual ~Window(void);

public:
	virtual bool init(const char *name, int x, int y, int width, int height, int flags);
	Uint32 id(void) const;
	SDL_Window *handle(void);
	const char *name(void) const;
	void rename(const char *name);
	int x(void) const;
	int y(void) const;
	const glm::ivec2 &pos(void) const;
	void move(int x, int y);
	int width(void) const;
	int height(void) const;
	const glm::ivec2 &size(void) const;
	void resize(int width, int height);
	const glm::ivec4 &rect(void) const;
	void visibled(void) const;
	void show(void);
	void hide(void);
	virtual bool handleEvent(const SDL_WindowEvent &event);
	virtual void update(float elapsed);

protected:
	void release(void);
	virtual void paint(void);

private:
	SDL_Window *m_window;
	Uint32 m_id;
	glm::ivec4 m_rect;
	bool m_visibled;
};

inline Uint32 Window::id(void) const
{
	SGL_ASSERT(m_window != NULL);
	return m_id;
}

inline SDL_Window *Window::handle(void)
{
	SGL_ASSERT(M_window != NULL);
	return m_window;
}

inline const char *Window::name(void) const
{
	SGL_ASSERT(m_window != NULL);
	return SDL_GetWindowTitle(m_window);
}

inline void Window::rename(const char *name)
{
	SGL_ASSERT(m_window != NULL);
	SDL_SetWindowTitle(m_window, name);
}

inline int Window::x(void) const
{
	SGL_ASSERT(m_window != NULL);
	return m_rect[0];
}

inline int y(void) const
{
	SGL_ASSERT(m_window != NULL);
	return m_rect[1];
}

inline const glm::ivec2 &Window::pos(void) const
{
	SGL_ASSERT(m_window != NULL);
	return glm::ivec2(m_rect[0], m_rect[1]);
}

inline void Window::move(int x, int y)
{
	SGL_ASSERT(m_window != NULL);
	SDL_SetWindowPosition(m_window, x, y);
}

inline int Window::width(void) const
{
	SGL_ASSERT(m_window != NULL);
	return m_rect[2];
}

inline int Window::height(void) const
{
	SGL_ASSERT(m_window != NULL);
	return m_rect[3];
}

inline const glm::ivec2 &Window::size(void) const
{
	SGL_ASSERT(m_window != NULL);
	return glm::ivec2(m_rect[2], m_rect[3]);
}

inline void Window::resize(int width, int height)
{
	SGL_ASSERT(m_window != NULL);
	SGL_SetWindowSize(m_window, width, height);
}

inline const glm::ivec4 &Window::rect(void) const
{
	SGL_ASSERT(m_window != NULL);
	return m_rect;
}

inline void Window::visibled(void) const
{
	SGL_ASSERT(m_window != NULL);
	return m_visibled;
}

inline void Window::show(void)
{
	SGL_ASSERT(m_window != NULL);
	SDL_ShowWindow(m_window);
}

inline void Window::hide(void)
{
	SGL_ASSERT(m_window != NULL);
	SDL_HideWindow(m_window);
}

SGL_END

#endif // SGL_WINDOW_HPP

