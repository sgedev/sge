//
//
#ifndef SGE_GL_CONTEXT_HPP
#define SGE_GL_CONTEXT_HPP

#include <sge/noncopyable.hpp>
#include <sge/gl/common.hpp>
#include <sge/gl/window.hpp>

SGE_GL_BEGIN

class context: public noncopyable {
public:
	context(void);
	virtual ~context(void);

public:
	bool init(window *w);
	void shutdown(void);
	bool make_current(void);
	void swap_buffers(void);
	window *get_window(void);
	SDL_GLContext to_sdl_gl_context(void);

private:
	window *m_window;
	SDL_GLContext m_context;
};

inline bool context::make_current(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_context != NULL);

	if (!m_window->visibled())
		return false;

	if (SDL_GL_MakeCurrent(m_window->to_sdl_window(), m_context) < 0)
		return false;

	return true;
}

inline void context::swap_buffers(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_context != NULL);
	SGE_ASSERT(m_context == SDL_GL_GetCurrentContext());

	SDL_GL_SwapWindow(m_window->to_sdl_window());
}

inline window *context::get_window(void)
{
	SGE_ASSERT(m_window != NULL);

	return m_window;
}

SDL_GLContext context::to_sdl_gl_context(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_context != NULL);

	return m_context;
}

SGE_GL_END

#endif // SGE_GL_CONTEXT_HPP
