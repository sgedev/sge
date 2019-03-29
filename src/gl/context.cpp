//
//
#include <sge/gl/context.hpp>

SGE_GL_BEGIN

static const char *debug_source(GLenum source)
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

static const char *debug_type(GLenum type)
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

static const char *debug_severity(GLenum severity)
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
	SGE_LOGD("[GL-%04d|%s|%s|%s] %s", id,
		debug_source(source), debug_type(type), debug_severity(severity), message);
}

context::context(void)
{
}

context::~context(void)
{
	if (m_window != NULL)
		shutdown();
}

bool context::init(void)
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

#ifdef SGE_DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	m_window = SDL_CreateWindow(SGE_GAME_NAME,
		SDL_WINDOWPOS_CENTERED,	SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (m_window == NULL)
		return false;

	SGE_LOGD("Initializing OpenGL...\n");

	m_context = SDL_GL_CreateContext(m_window);
	if (m_context == NULL) {
		SDL_DestroyWindow(m_window);
		SGE_LOGE("Failed to create OpenGL context.\n");
		return false;
	}

	SDL_GL_MakeCurrent(m_window, m_context);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_GL_DeleteContext(m_context);
		SDL_DestroyWindow(m_window);
		SGE_LOGE("Failed to initialize GLEW.\n");
		return false;
	}

#ifdef SGE_DEBUG
	if (GL_KHR_debug) {
		SGE_LOGD("Enable OpenGL debug output.\n");
		glDebugMessageCallback(debug_output, NULL);
	}
#endif

	SGE_LOGI("OpenGL: %s\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(0.0f);

	m_window_id = SDL_GetWindowID(m_window);

	return true;
}

void context::shutdown(void)
{
	printf("%s(%d)\n", __func__, __LINE__);
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_context != NULL);

	SGE_LOGI("Shutding down OpenGL...\n");

	if (SDL_GL_GetCurrentContext() == m_context)
		SDL_GL_MakeCurrent(m_window, NULL);

	SGE_LOGD("Destroy OpenGL context...\n");
	SDL_GL_DeleteContext(m_context);
	m_context = NULL;

	SGE_LOGD("Destroy OpenGL window...\n");
	SDL_DestroyWindow(m_window);
	m_window = NULL;
}

SGE_GL_END

