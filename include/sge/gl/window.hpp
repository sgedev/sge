//
//
#ifndef SGE_GL_WINDOW_HPP
#define SGE_GL_WINDOW_HPP

#include <sge/noncopyable.hpp>
#include <sge/gl/common.hpp>

SGE_GL_BEGIN

class window : public noncopyable {
public:
	window(void);
	virtual ~window(void);

public:
	bool create(const char *name, int x, int y, int width, int height);
	void destroy(void);
	bool handle_event(const SDL_Event &event);
	void show(void);
	void hide(void);
	bool visibled(void) const;
	void move(int x, int y);
	void resize(int width, int height);
	Uint32 id(void) const;
	glm::ivec2 pos(void) const;
	glm::ivec2 size(void) const;
	glm::ivec4 rect(void) const;
	SDL_Window *to_sdl_window(void);

private:
	SDL_Window *m_window;
	Uint32 m_id;
	int m_rect[4];
	bool m_visibled;
};

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

inline bool window::visibled(void) const
{
	SGE_ASSERT(m_window != NULL);

	return m_visibled;
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

inline Uint32 window::id(void) const
{
	SGE_ASSERT(m_window != NULL);

	return m_id;
}

inline glm::ivec2 window::pos(void) const
{
	SGE_ASSERT(m_window != NULL);

	return glm::ivec2(m_rect[0], m_rect[1]);
}

inline glm::ivec2 window::size(void) const
{
	SGE_ASSERT(m_window != NULL);

	return glm::ivec2(m_rect[2], m_rect[3]);
}

inline glm::ivec4 window::rect(void) const
{
	SGE_ASSERT(m_window != NULL);

	return glm::ivec4(m_rect[0], m_rect[1], m_rect[2], m_rect[3]);
}

inline SDL_Window *window::to_sdl_window(void)
{
	SGE_ASSERT(m_window != NULL);

	return m_window;
}

SGE_GL_END

#endif // SGE_GL_WINDOW_HPP
