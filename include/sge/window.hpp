//
//
#ifndef SGE_WINDOW_HPP
#define SGE_WINDOW_HPP

#include <string>

#include <sge/common.hpp>

SGE_BEGIN

class window {
public:
	window(void);
	virtual ~window(void);

public:
	bool init(void);
	void shutdown(void);
	void handle_event(const SDL_Event &event);
	bool draw_begin(void);
	void draw_end(void);
	SDL_Window *to_sdl_window(void);
	SDL_GLContext sdl_gl_context(void);
	Uint32 sdl_id(void) const;
	const glm::ivec4 &rect(void) const;
	void move(int x, int y);
	void resize(int width, int height);
	bool visibled(void) const;
	void show(void);
	void hide(void);
	bool fullscreen(void) const;
	const char *title(void) const;
	void set_title(const char *title);

private:
	enum {
		FLAG_VISIBLED = 0x1,
		FLAG_FULLSCREEN = 0x2,
	};

	SDL_Window *m_window;
	glm::ivec4 m_rect;
	Uint32 m_id;
	SDL_GLContext m_gl_context;
	int m_flags;
	std::string m_title;
};

inline void window::draw_end(void)
{
	SGE_ASSERT(m_window != NULL);
	SDL_GL_SwapWindow(m_window);
}

inline SDL_Window *window::to_sdl_window(void)
{
	SGE_ASSERT(m_window != NULL);
	return m_window;
}

inline SDL_GLContext window::sdl_gl_context(void)
{
	SGE_ASSERT(m_window != NULL);
	return m_gl_context;
}

inline Uint32 window::sdl_id(void) const
{
	SGE_ASSERT(m_window != NULL);
	return m_id;
}

inline const glm::ivec4 &window::rect(void) const
{
	SGE_ASSERT(m_window != NULL);
	return m_rect;
}

inline void window::move(int x, int y)
{
	SGE_ASSERT(m_window != NULL);
	SDL_SetWindowPosition(m_window, x, y);
}

inline void window::resize(int width, int height)
{
	SGE_ASSERT(m_window != NULL);
	SDL_SetWindowSize(m_window, width, height);
}

inline bool window::visibled(void) const
{
	SGE_ASSERT(m_window != NULL);
	return (m_flags & FLAG_VISIBLED);
}

inline void window::show(void)
{
	SGE_ASSERT(m_window != NULL);
	SDL_ShowWindow(m_window);
}

inline void window::hide(void)
{
	SGE_ASSERT(m_window != NULL);
	SDL_HideWindow(m_window);
}

inline bool window::fullscreen(void) const
{
	SGE_ASSERT(m_window != NULL);
	return (m_flags & FLAG_FULLSCREEN);
}

inline const char *window::title(void) const
{
	SGE_ASSERT(m_window != NULL);
	return m_title.c_str();
}

inline void window::set_title(const char *title)
{
	SGE_ASSERT(m_window != NULL);
	m_title = title;
	SDL_SetWindowTitle(m_window, title);
}

SGE_END

#endif // SGE_WINDOW_HPP

