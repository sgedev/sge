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
	class object {
	public:
		object(void);

	public:
		void init(void);
		void shutdown(void);

	protected:
		bool check_context(void) const;

	private:
		context *m_owner;
	};

public:
	context(void);
	virtual ~context(void);

public:
	bool create(window &win);
	void destroy(void);
	bool make_current(void);
	void swap_buffers(void);
	window *get_window(void);
	SDL_GLContext to_sdl_gl_context(void);
	static context *current(void);

private:
	window *m_window;
	SDL_GLContext m_context;
	static context *c_current;
};

inline context::object::object(void)
	: m_owner(NULL)
{
}

inline void context::object::init(void)
{
	SGE_ASSERT(m_owner == NULL);

	m_owner = c_current;
}

inline void context::object::shutdown(void)
{
	SGE_ASSERT(m_owner != NULL);

	m_owner = NULL;
}

inline bool context::object::check_context(void) const
{
	SGE_ASSERT(m_owner != NULL);

	return m_owner == context::c_current;
}

inline window *context::get_window(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_context != NULL);

	return m_window;
}

inline SDL_GLContext context::to_sdl_gl_context(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_context != NULL);

	return m_context;
}

inline bool context::make_current(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_context != NULL);

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

SGE_GL_END

#endif // SGE_GL_CONTEXT_HPP
