//
//
#include <sge/gl.hpp>

SGE_GL_BEGIN

static SDL_Window *s_window;
static Uint32 s_window_id;
static glm::ivec4 s_window_rect;
static bool s_window_visibled;
static SDL_GLContext s_context;
static bool s_fullscreen;

#ifdef SGE_DEBUG

static inline const char *debug_source(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:             return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "WINDOW_SYSTEM";
	case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER_COMPILER";
	case GL_DEBUG_SOURCE_THIRD_PARTY:     return "THIRD_PART";
	case GL_DEBUG_SOURCE_APPLICATION:     return "APPLICATION";
	case GL_DEBUG_SOURCE_OTHER:           return "OTHER";
	}
	return "UNKNOWN_SOURCE";
}

static inline const char *debug_type(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:               return "ERROR";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
	case GL_DEBUG_TYPE_PORTABILITY:         return "PORTABILITY";
	case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
	case GL_DEBUG_TYPE_MARKER:              return "MARKER";
	case GL_DEBUG_TYPE_PUSH_GROUP:          return "PUSH_GROUP";
	case GL_DEBUG_TYPE_POP_GROUP:           return "POP_GROUP";
	case GL_DEBUG_TYPE_OTHER:               return "OTHER";
	}
	return "UNKNOWN_TYPE";
}

static inline const char *debug_severity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
	case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
	case GL_DEBUG_SEVERITY_LOW:          return "LOW";
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
	}
	return "UNKNOWN_SEVERITY";
}

static void debug_output(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar *message, const void *data)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		SGE_LOGE("GL-%04d|%s|%s|%s] %s", id,
			debug_source(source), debug_type(type), debug_severity(severity), message);
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
	case GL_DEBUG_TYPE_PORTABILITY:
	case GL_DEBUG_TYPE_PERFORMANCE:
		SGE_LOGW("GL-%04d|%s|%s|%s] %s", id,
			debug_source(source), debug_type(type), debug_severity(severity), message);
		break;
	default:
		SGE_LOGD("GL-%04d|%s|%s|%s] %s", id,
			debug_source(source), debug_type(type), debug_severity(severity), message);
		break;
	}
}

#endif // SGE_DEBUG

bool init(void)
{
	SGE_ASSERT(s_window == NULL);
	SGE_ASSERT(s_context == NULL);

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

	s_window = SDL_CreateWindow("SGE",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

	if (s_window == NULL)
		goto bad0;

	s_fullscreen = false;
	s_window_id = SDL_GetWindowID(s_window);
	SDL_GetWindowPosition(s_window, &s_window_rect[0], &s_window_rect[1]);
	SDL_GetWindowSize(s_window, &s_window_rect[2], &s_window_rect[3]);

	SGE_LOGD("Initializing OpenGL...\n");

	s_context = SDL_GL_CreateContext(s_window);
	if (s_context == NULL) {
		SGE_LOGE("Failed to create OpenGL context.\n");
		goto bad1;
	}

	SDL_GL_MakeCurrent(s_window, s_context);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		SGE_LOGE("Failed to initialize GLEW.\n");
		goto bad2;
	}

	SGE_LOGI("GLEW: %s\n", glewGetString(GLEW_VERSION));
	SGE_LOGI("OpenGL: %s\n", glGetString(GL_VERSION));

#ifdef SGE_DEBUG
	if (GL_KHR_debug) {
		SGE_LOGD("Enable OpenGL debug output.\n");
		glDebugMessageCallback(debug_output, NULL);
	}
#endif

	return true;

bad2:
	SDL_GL_DeleteContext(s_context);
	s_context = NULL;

bad1:
	SDL_DestroyWindow(s_window);
	s_window = NULL;
	s_window_id = 0;

bad0:
	return false;
}

void shutdown(void)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_context != NULL);

	if (s_context == SDL_GL_GetCurrentContext())
		SDL_GL_MakeCurrent(s_window, NULL);

	SDL_GL_DeleteContext(s_context);
	s_context = NULL;

	SDL_DestroyWindow(s_window);
	s_window = NULL;
	s_window_id = 0;
}

bool handle_event(const SDL_Event &event)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_context != NULL);

	if (event.window.windowID != s_window_id)
		return false;

	switch (event.type) {
	case SDL_WINDOWEVENT:
		switch (event.window.event) {
		case SDL_WINDOWEVENT_MOVED:
			s_window_rect.x = event.window.data1;
			s_window_rect.y = event.window.data2;
			return true;
		case SDL_WINDOWEVENT_RESIZED:
			s_window_rect.z = event.window.data1;
			s_window_rect.w = event.window.data2;
			return true;
		case SDL_WINDOWEVENT_EXPOSED:
		case SDL_WINDOWEVENT_SHOWN:
			s_window_visibled = true;
			return true;
		case SDL_WINDOWEVENT_HIDDEN:
		case SDL_WINDOWEVENT_MINIMIZED:
			s_window_visibled = false;
			return true;
		}
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_RETURN && event.key.keysym.mod & KMOD_ALT) {
			s_fullscreen = !s_fullscreen;
			if (s_fullscreen)
				SDL_SetWindowFullscreen(s_window, SDL_WINDOW_FULLSCREEN);
			else
				SDL_SetWindowFullscreen(s_window, 0);
			return true;
		}
		break;
	}

	return false;
}

bool make_current(void)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_context != NULL);

	if (!s_window_visibled)
		return false;

	if (s_window_rect[2] < 1 || s_window_rect[3] < 1)
		return false;

	if (SDL_GL_MakeCurrent(s_window, s_context) != 0)
		return false;

	glViewport(0, 0, s_window_rect[2], s_window_rect[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	return true;
}

void swap_buffers(void)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_window == SDL_GL_GetCurrentWindow());
	SGE_ASSERT(s_context != NULL);
	SGE_ASSERT(s_context == SDL_GL_GetCurrentContext());

	SDL_GL_SwapWindow(s_window);
}

SDL_Window *window(void)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_context != NULL);

	return s_window;
}

SDL_GLContext context(void)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_context != NULL);

	return s_context;
}

SGE_GL_END

