//
//
#ifndef SGE_GL_CONTEXT_HPP
#define SGE_GL_CONTEXT_HPP

#include <sge/gl/common.hpp>
#include <sge/gl/program.hpp>

SGE_GL_BEGIN

class context {
public:
	context(void);
	virtual ~context(void);

public:
	bool init(SDL_Window *w);
	void shutdown(void);
	bool make_current(void);
	void swap_buffers(void);
	SDL_GLContext to_sdl(void);
	SDL_Window *get_window(void);
	static context *get_current(void);

private:
	SDL_Window *m_window;
	SDL_GLContext m_gl_context;
	static context *c_current;
};

inline bool context::make_current(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_gl_context != NULL);
	SGE_ASSERT(c_current != this);
	SGE_ASSERT(SDL_GL_GetCurrentContext() != m_gl_context);

	if (SDL_GL_MakeCurrent(m_window, m_gl_context) < 0)
		return false;

	c_current = this;

	return true;
}

inline void context::swap_buffers(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(c_current == this);
	SGE_ASSERT(m_gl_context == SDL_GL_GetCurrentContext());

	SDL_GL_SwapWindow(m_window);
	c_current = NULL;
}

inline SDL_GLContext context::to_sdl(void)
{
	SGE_ASSERT(m_gl_context != NULL);

	return m_gl_context;
}

inline SDL_Window *context::get_window(void)
{
	SGE_ASSERT(m_window != NULL);

	return m_window;
}

SGE_GL_END

#endif // SGE_GL_CONTEXT_HPP

