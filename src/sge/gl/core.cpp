//
//
#include <sge/gl.hpp>

SGE_GL_BEGIN

static SDL_Window *window;
static SDL_GLContext gl_context;
static Uint32 window_id;
static bool visibled;
static bool began;

bool init(void)
{
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

	window = SDL_CreateWindow(SGE_GAME_NAME,
		SDL_WINDOWPOS_CENTERED,	SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

	if (window == NULL)
		return false;

	SGE_LOGD("Initializing OpenGL...\n");

	gl_context = SDL_GL_CreateContext(window);
	if (gl_context == NULL) {
		SDL_DestroyWindow(window);
		SGE_LOGE("Failed to create OpenGL context.\n");
		return false;
	}

	SDL_GL_MakeCurrent(window, gl_context);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
		SGE_LOGE("Failed to initialize GLEW.\n");
		return false;
	}

#ifdef SGE_DEBUG
	//if (GL_KHR_debug) {
	//	SGE_LOGD("Enable OpenGL debug output.\n");
	//	glDebugMessageCallback(debug_output, NULL);
	//}
#endif

	SGE_LOGI("OpenGL: %s\n", glGetString(GL_VERSION));

	window_id = SDL_GetWindowID(window);

	return true;
}

void shutdown(void)
{
	SGE_ASSERT(window != NULL);
	SGE_ASSERT(gl_context != NULL);

	if (gl_context == SDL_GL_GetCurrentContext())
		SDL_GL_MakeCurrent(window, NULL);

	SDL_GL_DeleteContext(gl_context);
	gl_context = NULL;

	SDL_DestroyWindow(window);
	window = NULL;
}

SDL_Window *get_window(void)
{
	SGE_ASSERT(window != NULL);

	return window;
}

SDL_GLContext get_context(void)
{
	SGE_ASSERT(window != NULL);
	SGE_ASSERT(gl_context != NULL);

	return gl_context;
}

void handle_event(const SDL_Event &event)
{
	if (event.type != SDL_WINDOWEVENT)
		return;

	if (event.window.windowID != window_id)
		return;

	switch (event.window.event) {
	case SDL_WINDOWEVENT_SHOWN:
		visibled = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		visibled = false;
		break;
	}
}

bool begin(void)
{
	SGE_ASSERT(!began);

	if (!visibled)
		return false;

	if (SDL_GL_MakeCurrent(window, gl_context))
		return false;

	began = true;

	return true;
}

void end(void)
{
	SGE_ASSERT(began);

	SDL_GL_SwapWindow(window);

	began = false;
}

SGE_GL_END

