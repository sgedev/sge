//
//
#include <sge/window.hpp>

SGE_BEGIN

window::window(void)
	: m_window(NULL)
	, m_gl(NULL)
{
}

window::~window(void)
{
	if (m_window != NULL)
		shutdown();
}

bool window::init(void)
{
	SGE_ASSERT(m_window == NULL);
	SGE_ASSERT(m_gl == NULL);

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

	m_flags = 0;
	m_title = "SGE";

	m_window = SDL_CreateWindow(m_title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (m_window == NULL)
		return false;

	m_id = SDL_GetWindowID(m_window);
	SDL_GetWindowPosition(m_window, &m_rect[0], &m_rect[1]);
	SDL_GetWindowSize(m_window, &m_rect[2], &m_rect[3]);

	SGE_LOGD("Creating OpenGL context...\n");

	m_gl = SDL_GL_CreateContext(m_window);
	if (m_gl == NULL) {
		SGE_LOGE("Failed to create OpenGL context.\n");
		SDL_DestroyWindow(m_window);
		m_window = NULL;
		return false;
	}

	if (SDL_GL_MakeCurrent(m_window, m_gl)) {
		SGE_LOGE("Failed to make current OpenGL context.\n");
		SDL_GL_DeleteContext(m_gl);
		m_gl = NULL;
		SDL_DestroyWindow(m_window);
		m_window = NULL;
		return false;
	}

	if (gl3wInit2(&m_gl3w, (GL3WGetProcAddressProc)SDL_GL_GetProcAddress) < 0) {
		SGE_LOGE("Failed to initialize glxw context.\n");
		SDL_GL_DeleteContext(m_gl);
		m_gl = NULL;
		SDL_DestroyWindow(m_window);
		m_window = NULL;
		return false;
	}

	return true;
}

void window::shutdown(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_gl != NULL);

	SDL_GL_DeleteContext(m_gl);
	m_gl = NULL;

	SDL_DestroyWindow(m_window);
	m_window = NULL;
}

bool window::handle_event(const SDL_Event *event)
{
	SGE_ASSERT(event != NULL);
	SGE_ASSERT(m_window != NULL);

	if (event->window.windowID != m_id)
		return false;

	switch (event->type) {
	case SDL_WINDOWEVENT:
		switch (event->window.event) {
		case SDL_WINDOWEVENT_MOVED:
			m_rect.x = event->window.data1;
			m_rect.y = event->window.data2;
			return true;
		case SDL_WINDOWEVENT_RESIZED:
			m_rect.z = event->window.data1;
			m_rect.w = event->window.data2;
			return true;
		case SDL_WINDOWEVENT_EXPOSED:
		case SDL_WINDOWEVENT_SHOWN:
			m_flags |= FLAG_VISIBLED;
			return true;
		case SDL_WINDOWEVENT_HIDDEN:
		case SDL_WINDOWEVENT_MINIMIZED:
			m_flags &= ~FLAG_VISIBLED;
			return true;
		}
		break;
	case SDL_KEYDOWN:
		if (event->key.keysym.sym == SDLK_RETURN && event->key.keysym.mod & KMOD_ALT) {
			if (m_flags & FLAG_FULLSCREEN) {
				m_flags &= ~FLAG_FULLSCREEN;
				SDL_SetWindowFullscreen(m_window, 0);
			} else {
				m_flags |= FLAG_FULLSCREEN;
				SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
			}
			return true;
		}
		break;
	}

	return false;
}

SGE_END
