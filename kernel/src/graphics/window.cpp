//
//
#include <sge/graphics/window.hpp>

SGE_GRAPHICS_BEGIN

window::window(void):
	m_handle(nullptr),
	m_id(0),
	m_gl_context(nullptr)
{
}

window::~window(void)
{
    shutdown();
}

bool window::init(const char *name, int width, int height, int flags)
{
	SGE_ASSERT(m_handle == nullptr);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef SGE_DEBUG
	int gl_flags;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &gl_flags);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, gl_flags | SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    m_handle = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
        flags | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	if (m_handle == nullptr)
		goto bad0;

	m_id = SDL_GetWindowID(m_handle);
	m_visible = true;

	{
		int width, height;
		SDL_GetWindowSize(m_handle, &width, &height);
		resize(width, height);
	}

	SGE_ASSERT(m_gl_context == nullptr);
	m_gl_context = SDL_GL_CreateContext(m_handle);
	if (m_gl_context == nullptr)
		goto bad1;

	if (SDL_GL_MakeCurrent(m_handle, m_gl_context) < 0)
		goto bad2;

	if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)SDL_GL_GetProcAddress) != GL3W_OK)
		goto bad2;

	gl3wProcs = &m_gl3w;

	return true;

bad2:
	SDL_GL_DeleteContext(m_gl_context);
	m_gl_context = nullptr;

bad1:
	SDL_DestroyWindow(m_handle);
	m_handle = nullptr;

bad0:
	return false;
}

void window::shutdown(void)
{
    if (m_gl_context != nullptr) {
        SDL_GL_DeleteContext(m_gl_context);
        m_gl_context = nullptr;
    }

    if (m_handle != nullptr) {
        SDL_DestroyWindow(m_handle);
        m_handle = nullptr;
    }
}

void window::handle_event(const SDL_WindowEvent &evt)
{
	SGE_ASSERT(m_handle != nullptr);

	if (evt.windowID != m_id)
		return;

	switch (evt.event) {
	case SDL_WINDOWEVENT_SHOWN:
		m_visible = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		m_visible = false;
	case SDL_WINDOWEVENT_RESIZED:
		resize(evt.data1, evt.data2);
		break;
	}
}

void window::resize(int width, int height)
{
	// TODO
	canvas::resize(width, height);
}

bool window::begin(void)
{
	SGE_ASSERT(m_handle != nullptr);
	SGE_ASSERT(m_gl_context != nullptr);

    if (!canvas::begin())
        return false;

    if (width() < 1 || height() < 1) {
        canvas::end();
		return false;
    }

    if (SDL_GL_MakeCurrent(m_handle, m_gl_context) < 0) {
        canvas::end();
		return false;
    }

	gl3wProcs = &m_gl3w;

    return true;
}

void window::end(void)
{
	SGE_ASSERT(m_handle != nullptr);
	SGE_ASSERT(m_gl_context != nullptr);
	SGE_ASSERT(SDL_GL_GetCurrentContext() == m_gl_context);
	SGE_ASSERT(gl3wProcs == &m_gl3w);

	gl3wProcs = nullptr;
	SDL_GL_SwapWindow(m_handle);
	canvas::end();
}

SGE_GRAPHICS_END
