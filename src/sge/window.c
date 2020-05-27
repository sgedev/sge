/*
 *
 */
#include <GL/glew.h>

#include <sge/window.h>

#define SGE_WINDOW_VISIBLE 0x1
#define SGE_WINDOW_FOCUS 0x2

static SDL_Window *sge_window;
static Uint32 sge_window_id;
static int sge_window_flags;
static int sge_window_rect[4];
static SDL_GLContext *sge_window_gl;

static void sge_window_gl_debug_log(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const char *message, const GLvoid *userParam)
{
    SGE_LOGI("GL: %s\n", message);
}

void sge_window_export(lua_State *L)
{
	lua_newtable(L);

	lua_setglobal(L, "window");
}

bool sge_window_init(void)
{
#ifdef SGE_DEBUG
	int gl_flags;
#endif

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef SGE_DEBUG
	SGE_LOGI("OpenGL debug enabled.\n");
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &gl_flags);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, gl_flags | SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	SGE_LOGD("Creating main window...\n");
	SGE_ASSERT(sge_window == NULL);
	sge_window = SDL_CreateWindow("SGE",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (sge_window == NULL) {
		SGE_LOGE("Failed to create main window.\n");
		goto bad0;
	}

	sge_window_id = SDL_GetWindowID(sge_window);
	sge_window_flags = SGE_WINDOW_FOCUS | SGE_WINDOW_VISIBLE;

	SDL_GetWindowPosition(sge_window, &sge_window_rect[0], &sge_window_rect[1]);
	SDL_GetWindowSize(sge_window, &sge_window_rect[2], &sge_window_rect[3]);

	SGE_LOGD("Create OpenGL context...\n");
	SGE_ASSERT(sge_window_gl == NULL);
	sge_window_gl = SDL_GL_CreateContext(sge_window);
	if (sge_window_gl == NULL) {
		SGE_LOGE("Failed to create OpenGL context.\n");
		goto bad1;
	}

	if (SDL_GL_MakeCurrent(sge_window, sge_window_gl) < 0) {
		SGE_LOGE("Failed to make current OpenGL context.\n");
		goto bad2;
	}

	if (glewInit() != GLEW_OK) {
		SGE_LOGE("Failed to init GLEW.\n");
		goto bad2;
	}

	SGE_LOGI("GLEW: %s\n", glewGetString(GLEW_VERSION));
	SGE_LOGI("OpenGL: %s\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

#ifdef SGE_DEBUG
	if (GL_KHR_debug)
		glDebugMessageCallback(sge_window_gl_debug_log, NULL);
#endif

	return true;

bad2:
	SDL_GL_DeleteContext(sge_window_gl);
	sge_window_gl = NULL;

bad1:
	SDL_DestroyWindow(sge_window);
	sge_window = NULL;

bad0:
	return false;
}

void sge_window_shutdown(void)
{
	SGE_ASSERT(sge_window_gl != NULL);
	SDL_GL_DeleteContext(sge_window_gl);
	sge_window_gl = NULL;

	SGE_ASSERT(sge_window != NULL);
	SDL_DestroyWindow(sge_window);
	sge_window = NULL;
}

void sge_window_handle_event(const SDL_WindowEvent *evt)
{
	if (evt->windowID != sge_window_id)
		return;

	switch (evt->event) {
	case SDL_WINDOWEVENT_SHOWN:
		sge_window_flags |= SGE_WINDOW_VISIBLE;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		sge_window_flags &= ~SGE_WINDOW_VISIBLE;
		break;
	case SDL_WINDOWEVENT_MOVED:
		sge_window_rect[0] = evt->data1;
		sge_window_rect[1] = evt->data2;
		break;
	case SDL_WINDOWEVENT_RESIZED:
		sge_window_rect[2] = evt->data1;
		sge_window_rect[3] = evt->data2;
		break;
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		sge_window_flags |= SGE_WINDOW_FOCUS;
		break;
	case SDL_WINDOWEVENT_FOCUS_LOST:
		sge_window_flags &= ~SGE_WINDOW_FOCUS;
		break;
	default:
		break;
	}
}

bool sge_window_begin_draw(void)
{
	if (!(sge_window_flags & SGE_WINDOW_VISIBLE))
		return false;

	if (sge_window_rect[2] < 1 || sge_window_rect[3] < 1)
		return false;

	SGE_ASSERT(sge_window != NULL);
	SGE_ASSERT(sge_window_gl != NULL);

	if (SDL_GL_MakeCurrent(sge_window, sge_window_gl) < 0)
		return false;

	glViewport(0, 0, sge_window_rect[2], sge_window_rect[3]);

	return true;
}

void sge_window_end_draw(void)
{
	SGE_ASSERT(sge_window != NULL);
	SDL_GL_SwapWindow(sge_window);
}
