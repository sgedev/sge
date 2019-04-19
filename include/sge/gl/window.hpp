//
//
#ifndef SGE_GL_WINDOW_HPP
#define SGE_GL_WINDOW_HPP

#include <sge/gl/common.hpp>

SGE_GL_BEGIN

class window {
public:
	window(void);
	virtual ~window(void);

public:
	bool init(SDL_Window *w = NULL);
	void shutdown(void);
	void handle_event(const SDL_Event &event);
	SDL_Window *to_sdl_window(void);

private:
	bool m_owned;
	SDL_Window *m_window;
	Uint32 m_window_id;
	bool m_window_visibled;
};

inline SDL_Window *window::to_sdl_window(void)
{
	SGE_ASSERT(m_window != NULL);

	return m_window;
}

SGE_GL_END

#endif // SGE_GL_WINDOW_HPP

