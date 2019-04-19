//
//
#include <sge/gl/window.hpp>

SGE_GL_BEGIN

window::window(void)
	: m_owned(false)
	, m_window(NULL)
	, m_window_id(0)
	, m_window_visibled(false)
{
}

window::~window(void)
{
	if (m_window != NULL)
		shutdown();
}

bool window::init(SDL_Window *w)
{
	SGE_ASSERT(m_window == NULL);

	if (w == NULL) {
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
			SDL_WINDOWPOS_CENTERED,	SDL_WINDOWPOS_CENTERED, 800, 600,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

		if (m_window == NULL)
			return false;

		m_owned = true;
	} else {
		Uint32 flags = SDL_GetWindowFlags(w);
		if (!(flags & SDL_WINDOW_OPENGL))
			return false;
		m_window = w;
		m_owned = false;
	}

	m_window_id = SDL_GetWindowID(m_window);

	return true;
}

void window::shutdown(void)
{
	SGE_ASSERT(m_window != NULL);

	if (!m_owned)
		SDL_DestroyWindow(m_window);

	m_window = NULL;
	m_window_id = 0;
	m_window_visibled = false;
}

void window::handle_event(const SDL_Event &event)
{
	SGE_ASSERT(m_window != NULL);

	if (event.type != SDL_WINDOWEVENT)
		return;

	if (event.window.windowID != m_window_id)
		return;

	switch (event.window.event) {
	case SDL_WINDOWEVENT_SHOWN:
		m_window_visibled = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		m_window_visibled = false;
		break;
	}
}

SGE_GL_END

