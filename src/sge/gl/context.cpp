//
//
#include <sge/gl/context.hpp>

SGE_GL_BEGIN

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

context *context::c_current = NULL;

context::context(void)
	: m_window(NULL)
	, m_context(NULL)
{
}

context::~context(void)
{
	if (m_context != NULL)
		destroy();
}

bool context::create(window &win)
{
	SGE_ASSERT(m_window == NULL);
	SGE_ASSERT(m_context == NULL);

	m_context = SDL_GL_CreateContext(win.to_sdl_window());
	if (m_context == NULL) {
		SGE_LOGE("Failed to create OpenGL context.\n");
		return false;
	}

	SDL_GL_MakeCurrent(win.to_sdl_window(), m_context);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		SGE_LOGE("Failed to initialize GLEW.\n");
		SDL_GL_DeleteContext(m_context);
		m_context = NULL;
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

	m_window = &win;

	return true;
}

void context::destroy(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_context != NULL);

	if (SDL_GL_GetCurrentContext() == m_context)
		SDL_GL_MakeCurrent(NULL, NULL);

	SDL_GL_DeleteContext(m_context);
	m_context = NULL;

	m_window = NULL;
}

context *context::current(void)
{
	return c_current;
}

SGE_GL_END
