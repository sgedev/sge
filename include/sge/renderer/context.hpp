//
//
#ifndef SGE_RENDERER_CONTEXT_HPP
#define SGE_RENDERER_CONTEXT_HPP

#include <memory>
#include <list>

#include <sge/noncopyable.hpp>
#include <sge/renderer/common.hpp>
#include <sge/renderer/view.hpp>

SGE_RENDERER_BEGIN

class context: public noncopyable {
public:
	context(void);
	virtual ~context(void);

public:
	bool init(void);
	void shutdown(void);
	bool begin(void);
	void end(void);
	void handle_event(const SDL_Event &event);

private:
	SDL_Window *m_window;
	Uint32 m_window_id;
	bool m_visibled;
	SDL_GLContext m_gl_context;
};

inline bool context::begin(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_gl_context != NULL);

	int ret = SDL_GL_MakeCurrent(m_window, m_gl_context);
	if (ret)
		return false;

	return true;
}

inline void context::end(void)
{
	SGE_ASSERT(m_window != NULL);

	SDL_GL_SwapWindow(m_window);
}

SGE_RENDERER_END

#endif // SGE_RENDERER_CONTEXT_HPP

