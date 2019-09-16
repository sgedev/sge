//
//
#ifndef SGE_WINDOW_HPP
#define SGE_WINDOW_HPP

#include <string>

#include <cx/noncopyable.hpp>
#include <GL/gl3w.h>

#include <sge/common.hpp>

SGE_BEGIN

class window: public cx::noncopyable {
public:
	window(void);
	virtual ~window(void);

public:
	bool init(void);
	void shutdown(void);
	bool handle_event(const SDL_Event *event);
	bool make_current(void);
	void done_current(void);
	void swap_buffers(void);
	bool visibled(void) const;
	const glm::ivec4& rect(void) const;
	SDL_Window* sdl_window(void);
	SDL_GLContext sdl_gl_context(void);

private:
	enum {
		FLAG_VISIBLED = 0x1,
		FLAG_FULLSCREEN = 0x2,
	};

private:
	SDL_Window *m_window;
	glm::ivec4 m_rect;
	Uint32 m_id;
	SDL_GLContext m_gl;
	union GL3WProcs m_gl3w;
	int m_flags;
	std::string m_title;
};

inline bool window::make_current(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_gl != NULL);

	if (SDL_GL_MakeCurrent(m_window, m_gl))
		return false;

	gl3wProcs = &m_gl3w;

	return true;
}

inline void window::done_current(void)
{
	SGE_ASSERT(m_window != NULL);

	gl3wProcs = NULL;
	SDL_GL_MakeCurrent(m_window, NULL);
}

inline void window::swap_buffers(void)
{
	SGE_ASSERT(m_window != NULL);

	gl3wProcs = NULL;
	SDL_GL_SwapWindow(m_window);
}

inline bool window::visibled(void) const
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_gl != NULL);

	return (m_flags & FLAG_VISIBLED);
}

inline const glm::ivec4& window::rect(void) const
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_gl != NULL);

	return m_rect;
}

inline SDL_Window *window::sdl_window(void)
{
	SGE_ASSERT(m_window != NULL);

	return m_window;
}

inline SDL_GLContext window::sdl_gl_context(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_gl != NULL);

	return m_gl;
}

SGE_END

#endif // SGE_PLAYER_HPP

