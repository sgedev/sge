//
//
#ifndef SGE_GL_CONTEXT_HPP
#define SGE_GL_CONTEXT_HPP

#include <sge/gl/common.hpp>
#include <sge/gl/window.hpp>
#include <sge/gl/program.hpp>

SGE_GL_BEGIN

class context {
public:
	enum program_type {
		PROGRAM_DEFAULT = 0,
		PROGRAM_MAX
	};

public:
	context(void);
	virtual ~context(void);

public:
	bool init(SDL_Window *w);
	bool init(window &w);
	void shutdown(void);
	bool make_current(void);
	void swap_buffers(void);
	SDL_GLContext to_sdl_gl_context(void);
	static context *get_current(void);

private:
	bool init_programs(void);

private:
	SDL_Window *m_window;
	SDL_GLContext m_gl_context;
	bool m_drawing;
	program m_program_table[PROGRAM_MAX];
	static context *c_current;
	static const char *c_vs_sources[PROGRAM_MAX];
	static const char *c_fs_sources[PROGRAM_MAX];
};

inline bool context::init(window &w)
{
	return init(w.to_sdl_window());
}

inline bool context::make_current(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_gl_context != NULL);
	SGE_ASSERT(!m_drawing);

	if (SDL_GL_MakeCurrent(m_window, m_gl_context) < 0)
		return false;

	m_drawing = true;
	c_current = this;

	return true;
}

inline void context::swap_buffers(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_drawing);
	SGE_ASSERT(c_current == this);
	SGE_ASSERT(m_gl_context == SDL_GL_GetCurrentContext());

	SDL_GL_SwapWindow(m_window);

	m_drawing = false;
	c_current = NULL;
}

inline SDL_GLContext context::to_sdl_gl_context(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_gl_context != NULL);

	return m_gl_context;
}

SGE_GL_END

#endif // SGE_GL_CONTEXT_HPP

