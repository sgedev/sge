//
//
#include <GL/gl.hpp>

#include <sge/window.hpp>

SGE_BEGIN

window::window(void)
	: m_window(NULL)
	, m_gl_context(NULL)
	, m_flags(0)
{
}

window::~window(void)
{
}

bool window::init(void)
{
	SGE_ASSERT(m_window == NULL);
	SGE_ASSERT(m_gl_context == NULL);

	SGE_LOGD("creating opengl window...\n");

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
	m_title = "sge";

	m_window = SDL_CreateWindow(m_title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);

	if (m_window == NULL)
		return false;

	m_id = SDL_GetWindowID(m_window);
	SDL_GetWindowPosition(m_window, &m_rect[0], &m_rect[1]);
	SDL_GetWindowSize(m_window, &m_rect[2], &m_rect[3]);

	SGE_LOGD("initializing opengl...\n");

	m_gl_context = SDL_GL_CreateContext(m_window);
	if (m_gl_context == NULL) {
		SGE_LOGE("failed to create opengl context.\n");
		SDL_DestroyWindow(m_window);
		m_window = NULL;
		return false;
	}

	SDL_GL_MakeCurrent(m_window, m_gl_context);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		SGE_LOGE("failed to initialize glew.\n");
		SDL_GL_DeleteContext(m_gl_context);
		m_gl_context = NULL;
		SDL_DestroyWindow(m_window);
		m_window = NULL;
		return false;
	}

	SGE_LOGI("opengl: %s\n", glGetString(GL_VERSION));
	SGE_LOGI("glew: %s\n", glewGetString(GLEW_VERSION));

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	return true;
}

void window::shutdown(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_gl_context != NULL);

	if (m_gl_context == SDL_GL_GetCurrentContext())
		SDL_GL_MakeCurrent(m_window, NULL);

	SDL_GL_DeleteContext(m_gl_context);
	m_gl_context = NULL;

	SDL_DestroyWindow(m_window);
	m_window = NULL;
}

void window::handle_event(const SDL_Event &event)
{
	if (event.window.windowID != m_id)
		return;

	switch (event.type) {
	case SDL_WINDOWEVENT:
		switch (event.window.event) {
		case SDL_WINDOWEVENT_MOVED:
			m_rect.x = event.window.data1;
			m_rect.y = event.window.data2;
			break;
		case SDL_WINDOWEVENT_RESIZED:
			m_rect.z = event.window.data1;
			m_rect.w = event.window.data2;
			break;
		case SDL_WINDOWEVENT_EXPOSED:
		case SDL_WINDOWEVENT_SHOWN:
			m_flags |= FLAG_VISIBLED;
			break;
		case SDL_WINDOWEVENT_HIDDEN:
		case SDL_WINDOWEVENT_MINIMIZED:
			m_flags &= ~FLAG_VISIBLED;
			break;
		}
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_RETURN && event.key.keysym.mod & KMOD_ALT) {
			if (m_flags & FLAG_FULLSCREEN) {
				m_flags &= ~FLAG_FULLSCREEN;
				SDL_SetWindowFullscreen(m_window, 0);
			} else {
				m_flags |= FLAG_FULLSCREEN;
				SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
			}
		}
		break;
	}
}

bool window::draw_begin(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_gl_context != NULL);

	if (!visibled())
		return false;

	if (m_rect[2] < 1 || m_rect[3] < 1)
		return false;

	if (SDL_GL_MakeCurrent(m_window, m_gl_context))
		return false;

	glViewport(0, 0, m_rect[2], m_rect[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	return true;
}

SGE_END

