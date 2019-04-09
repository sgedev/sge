//
//
#ifndef SGE_RENDERER_CONTEXT_HPP
#define SGE_RENDERER_CONTEXT_HPP

#include <sge/renderer/common.hpp>
#include <sge/renderer/drawable.hpp>

SGE_RENDERER_BEGIN

class context {
public:
	context(void);
	virtual ~context(void);

public:
	bool init(void);
	void shutdown(void);
	void handle_event(const SDL_Event &event);
	bool begin(void);
	void end(void);
	void draw(drawable &d);

private:
	SDL_Window *m_window;
	Uint32 m_window_id;
	bool m_visibled;
	bool m_drawable;
	SDL_GLContext m_gl_context;
};

SGE_RENDERER_END

#endif // SGE_RENDERER_CONTEXT_HPP

