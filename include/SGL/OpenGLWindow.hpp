//
//
#ifndef SGL_OPENGLWINDOW_HPP
#define SGL_OPENGLWINDOW_HPP

#include <GL/gl3w.h>

#include <SGL/Common.hpp>
#include <SGL/Window.hpp>

SGL_BEGIN

class OpenGLWindow: public Window {
public:
	OpenGLWindow(void);
	virtual ~OpenGLWindow(void);

public:
	bool init(const char *name, int x, int y, int width, int height, int flags);
	SDL_GLContext context(void);
	bool handleEvent(const SDL_WindowEvent &event);
	void update(float elapsed);

protected:
	void paint(void) override;
	virtual bool initGL(void);

private:
	SDL_GLContext m_context;
	union GL3WProcs m_gl3w;
};

inline SDL_GLContext OpenGLWindow::context(void)
{
	SGL_ASSERT(m_context != NULL);
	return m_context;
}

SGL_END

#endif // SGL_OPENGLWINDOW_HPP

