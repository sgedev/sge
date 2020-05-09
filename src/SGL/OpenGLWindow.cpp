//
//
#include <SGL/OpenGLWindow.hpp>

SGL_BEGIN

OpenGLWindow::OpenGLWindow(void):
	m_gl(NULL)
{
}

OpenGLWindow::~OpenGLWindow(void)
{
	if (m_gl != NULL)
		SDL_GL_DeleteContext(m_gl);
}

bool OpenGLWindow::init(const char *name, int x, int y, int width, int height, int flags)
{
	SGL_ASSERT(m_gl == NULL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef SGL_DEBUG
	int flags;
	SGL_LOGI("OpenGL debug enabled.");
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &flags);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, flags | SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	if (!Window::init(name, x, y, width, height, flags | SDL_WINDOW_OPENGL)) {
		goto bad0;
	}	

	SGE_LOGD("Create OpenGL context...");
	m_gl = SDL_GL_CreateContext(handle());
	if (m_gl == NULL) {
		SGL_LOGE("Failed to create OpenGL context.");
		goto bad1;
	}

	SDL_GL_MakeCurrent(handle(), m_gl);

	if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)SDL_GL_GetProcAddress) < 0) {
		SGL_LOGE("Failed to init OpenGL procs.");
		goto bad2;
	}

	gl3wProcs = &m_gl3w;

	SGL_LOGI("GLEW: %s", glewGetString(GLEW_VERSION));
	SGL_LOGI("OpenGL: %s", glGetString(GL_VERSION));
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

#if defined SGE_DEBUG && defined SGE_LOG
	if (GL_KHR_debug)
		glDebugMessageCallback(sge_gl_debug_output, NULL);
#endif

	return true;

bad2:
	SDL_GL_DeleteContext(m_gl);
	m_gl = NULL;

bad1:
	release();

bad0:
	return false;
}

bool OpenGLWindow::handleEvent(const SDL_WindowEvent &event)
{
	bool ret = Window::handleEvent(event);

	// TODO

	return ret;
}

void OpenGLWindow::update(float elapsed)
{
	SGL_ASSERT(m_gl != NULL);

	if (!visibled())
		return;

	if (width() < 1 || height() < 1)
		return;

	if (SDL_GL_MakeCurrent(handle(), m_gl) < 0)
		return;

	gl3wProcs = &m_gl3w;

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	Window::update(elapsed);

	gl3wProcs = NULL;

	SDL_GL_SwapWindow(m_window);
}

SGL_END

