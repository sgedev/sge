//
//
#ifndef SGE_GL_CONTEXT_HPP
#define SGE_GL_CONTEXT_HPP

#include <SDL.h>

#include <sge/gl/common.hpp>

SGE_GL_BEGIN

class context {
public:
	context(void);
	virtual ~context(void);

public:
	bool init(void);
	void shutdown(void);
	void make_current(void);
	void swap_window(void);
	Uint32 get_window_id(void) const;

private:
	SDL_Window *m_window;
	SDL_GLContext m_context;
	Uint32 m_window_id;
};

inline void context::make_current(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_context != NULL);
	SDL_GL_MakeCurrent(m_window, m_context);
	glClear(GL_COLOR_BUFFER_BIT);
}

inline void context::swap_window(void)
{
	SGE_ASSERT(m_window != NULL);
	SDL_GL_SwapWindow(m_window);
}

inline Uint32 context::get_window_id(void) const
{
	SGE_ASSERT(m_window != NULL);
	return m_window_id;
}

SGE_GL_END

#endif // SGE_GL_CONTEXT_HPP

