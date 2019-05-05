//
//
#include <sge/gl/window.hpp>

SGE_GL_BEGIN

window::window(void)
	: m_window(NULL)
	, m_id(0)
	, m_visibled(false)
{
}

window::~window(void)
{
	if (m_window != NULL)
		destroy();
}

bool window::create(const char *name, int x, int y, int width, int height)
{
	SGE_ASSERT(m_window == NULL);

	SGE_LOGD("Creating OpenGL window...\n");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

#ifdef SGE_DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	m_window = SDL_CreateWindow("SGE", x, y, width, height,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

	if (m_window == NULL)
		return false;

	m_id = SDL_GetWindowID(m_window);
	SDL_GetWindowPosition(m_window, &m_rect[0], &m_rect[1]);
	SDL_GetWindowSize(m_window, &m_rect[2], &m_rect[3]);

	return true;
}

void window::destroy(void)
{
	SGE_ASSERT(m_window != NULL);

	SDL_DestroyWindow(m_window);
}

bool window::handle_event(const SDL_Event &event)
{
	SGE_ASSERT(m_window != NULL);

	if (event.type != SDL_WINDOWEVENT)
		return false;

	if (event.window.windowID != m_id)
		return false;

	switch (event.window.event) {
	case SDL_WINDOWEVENT_MOVED:
		m_rect[0] = event.window.data1;
		m_rect[1] = event.window.data2;
		return true;
	case SDL_WINDOWEVENT_RESIZED:
		m_rect[2] = event.window.data1;
		m_rect[3] = event.window.data2;
		return true;
	case SDL_WINDOWEVENT_EXPOSED:
	case SDL_WINDOWEVENT_SHOWN:
		m_visibled = true;
		return true;
	case SDL_WINDOWEVENT_HIDDEN:
	case SDL_WINDOWEVENT_MINIMIZED:
		m_visibled = false;
		return true;
	}

	return false;
}

SGE_GL_END
