//
//
#include <sge/gl/window.hpp>

SGE_GL_BEGIN

window::window(void)
	: m_window(NULL)
{
}

window::~window(void)
{
	if (m_window != NULL)
		shutdown();
}

bool window::init(SDL_Window *from)
{
	SGE_ASSERT(m_window == NULL);

	if (from == NULL) {
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

		m_window = SDL_CreateWindow("SGE",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
		m_owned = true;
	} else {
		if (!(SDL_GetWindowFlags(from) & SDL_WINDOW_OPENGL))
			return false;
		m_window = from;
		m_owned = false;
	}

	if (m_window == NULL)
		return false;

	m_id = SDL_GetWindowID(m_window);

	SDL_GetWindowPosition(m_window, &m_pos.x, &m_pos.y);
	SDL_GetWindowSize(m_window, &m_size.x, &m_size.y);

	return true;
}

void window::shutdown(void)
{
	SGE_ASSERT(m_window != NULL);

	SDL_DestroyWindow(m_window);
	m_window = NULL;
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
		m_pos.x = event.window.data1;
		m_pos.y = event.window.data2;
		return true;
	case SDL_WINDOWEVENT_RESIZED:
		m_size.x = event.window.data1;
		m_size.y = event.window.data2;
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
