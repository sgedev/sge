//
//
#include <sge/gl/context.hpp>

SGE_GL_BEGIN

static SDL_Window *s_window = NULL;
static Uint32 s_window_id = 0;
static glm::ivec2 s_window_pos;
static glm::ivec2 s_window_size;
static int s_window_height;
static bool s_window_visibled = false;
static SDL_GLContext s_context = NULL;

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
	return "";
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
	return "";
}

static inline const char *debug_severity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
	case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
	case GL_DEBUG_SEVERITY_LOW:          return "LOW";
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
	}
	return "";
}

static void debug_output(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar *message, const void *data)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		SGE_LOGE("GL-%04d|%s|%s|%s] %s", id,
			debug_source(source), debug_type(type), debug_severity(severity), message);
		break;
	default:
		SGE_LOGI("GL-%04d|%s|%s|%s] %s", id,
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

	s_window_pos = glm::ivec2(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	s_window_size = glm::ivec2(800, 600);

	s_window = SDL_CreateWindow("SGE",
		s_window_pos[0], s_window_pos[1], s_window_size[0], s_window_size[1],
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

	if (s_window == NULL)
		return false;

	s_window_id = SDL_GetWindowID(s_window);

	SGE_LOGD("Initializing OpenGL...\n");

	s_context = SDL_GL_CreateContext(s_window);
	if (s_context == NULL) {
		SGE_LOGE("Failed to create OpenGL context.\n");
		SDL_DestroyWindow(s_window);
		s_window = NULL;
		return false;
	}

	SDL_GL_MakeCurrent(s_window, s_context);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		SGE_LOGE("Failed to initialize GLEW.\n");
		SDL_GL_DeleteContext(s_context);
		s_context = NULL;
		SDL_DestroyWindow(s_window);
		s_window = NULL;
		return false;
	}

#ifdef SGE_DEBUG
	if (GL_KHR_debug) {
		SGE_LOGD("Enable OpenGL debug output.\n");
		glDebugMessageCallback(debug_output, NULL);
	}
#endif

	SGE_LOGI("GLEW: %s\n", glewGetString(GLEW_VERSION));
	SGE_LOGI("OpenGL: %s\n", glGetString(GL_VERSION));

	return true;
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

bool make_current(void)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_context != NULL);

	if (!s_window_visibled)
		return false;

	int ret = SDL_GL_MakeCurrent(s_window, s_context);
	if (ret < 0)
		return false;

	glViewport(0, 0, s_window_size[0], s_window_size[1]);

	return true;
}

void swap_buffers(void)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_context != NULL);
	SGE_ASSERT(s_context == SDL_GL_GetCurrentContext());

	SDL_GL_SwapWindow(s_window);
}

void handle_event(const SDL_Event &event)
{
	SGE_ASSERT(s_window != NULL);

	if (event.type != SDL_WINDOWEVENT)
		return;

	if (event.window.windowID != s_window_id)
		return;

	switch (event.window.event) {
	case SDL_WINDOWEVENT_MOVED:
		s_window_pos = glm::ivec2(event.window.data1, event.window.data2);
		break;
	case SDL_WINDOWEVENT_RESIZED:
		s_window_size = glm::ivec2(event.window.data1, event.window.data2);
		break;
	case SDL_WINDOWEVENT_EXPOSED:
	case SDL_WINDOWEVENT_SHOWN:
		s_window_visibled = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
	case SDL_WINDOWEVENT_MINIMIZED:
		s_window_visibled = false;
		break;
	}
}

SDL_Window *window(void)
{
	SGE_ASSERT(s_window != NULL);

	return s_window;
}

Uint32 window_id(void)
{
	SGE_ASSERT(s_window != NULL);

	return s_window_id;
}

const glm::ivec2 &window_pos(void)
{
	SGE_ASSERT(s_window != NULL);

	return s_window_pos;
}

const glm::ivec2 &window_size(void)
{
	SGE_ASSERT(s_window != NULL);

	return s_window_size;
}

SDL_GLContext context(void)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_context != NULL);

	return s_context;
}

SGE_GL_END

