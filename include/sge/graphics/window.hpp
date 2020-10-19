//
//
#ifndef SGE_GRAPHICS_WINDOW_HPP
#define SGE_GRAPHICS_WINDOW_HPP

#include <sge/graphics/common.hpp>
#include <sge/graphics/canvas.hpp>

SGE_GRAPHICS_BEGIN

class window: public canvas {
public:
	window(void);
	~window(void) override;

public:
    bool init(const char *name, int width, int height, int flags = 0);
    void shutdown(void);
	void handle_event(const SDL_WindowEvent &evt);
	Uint32 id(void) const;
	bool visible(void) const;
	void show(void);
	void hide(void);
	void resize(int width, int height) override;
	bool begin(void) override;
	void end(void) override;

private:
	SDL_Window *m_handle;
	Uint32 m_id;
	bool m_visible;
	SDL_GLContext m_gl_context;
	union GL3WProcs m_gl3w;
};

SGE_INLINE Uint32 window::id(void) const
{
	return m_id;
}

SGE_INLINE bool window::visible(void) const
{
	return m_visible;
}

SGE_INLINE void window::show(void)
{
	SGE_ASSERT(m_handle != nullptr);
	SDL_ShowWindow(m_handle);
}

SGE_INLINE void window::hide(void)
{
	SGE_ASSERT(m_handle != nullptr);
	SDL_HideWindow(m_handle);
}

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_WINDOW_HPP
