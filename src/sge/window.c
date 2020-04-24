/*
 *
 */
#include <sge/window.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

#define SGE_WINDOW_VISIBLE 0x1
#define SGE_WINDOW_FOCUS 0x2

static SDL_Window *sge_window;
static Uint32 sge_window_id;
static int sge_window_width;
static int sge_window_height;
static int sge_window_flags;
static SDL_GLContext sge_window_gl;
static GLEXContext *sge_window_glex;
static NVGcontext *sge_window_nvg;

#if defined SGE_DEBUG && defined SGE_LOG

static const char *sge_window_gl_debug_source(GLenum source)
{
	const char *ret;

	switch (source) {
	case GL_DEBUG_SOURCE_API:
		ret = "API";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		ret = "GLSL";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		ret = "WINDOW_SYSTEM";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		ret = "THIRD_PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		ret = "APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		ret = "OTHER";
		break;
	default:
		ret = "UNKNOWN";
		break;
	}

	return ret;
}

static const char *sge_window_gl_debug_type(GLenum type)
{
	const char *ret;

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		ret = "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		ret = "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		ret = "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		ret = "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		ret = "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_MARKER:
		ret = "MARKER";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		ret = "PUSH_GROUP";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		ret = "POP_GROUP";
		break;
	case GL_DEBUG_TYPE_OTHER:
		ret = "OTHER";
		break;
	default:
		ret = "UNKNOWN";
		break;
	}

	return ret;
}

static const char *sge_window_gl_debug_serverity(GLenum severity)
{
	const char *ret;

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		ret = "H";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		ret = "M";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		ret = "L";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		ret = "N";
		break;
	default:
		ret = "X";
		break;
	}

	return ret;
}

static void sge_window_gl_debug_output(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar *message, const void *data)
{
	if (type == GL_DEBUG_TYPE_ERROR) {
		SGE_LOGE("GL-%s-%s-%s: %s\n",
			sge_window_gl_debug_serverity(severity),
			sge_window_gl_debug_source(source),
			sge_window_gl_debug_type(type), message);
	} else {
		SGE_LOGD("GL-%s-%s-%s: %s\n",
			sge_window_gl_debug_serverity(severity),
			sge_window_gl_debug_source(source),
			sge_window_gl_debug_type(type), message);
	}
}

#endif /* SGE_DEBUG && SGE_LOG */

int sge_window_init(void)
{
	int flags;

	CX_ASSERT(sge_window == NULL);
	CX_ASSERT(sge_window_gl == NULL);
	CX_ASSERT(sge_window_glex == NULL);
	CX_ASSERT(sge_window_nvg == NULL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef SGE_DEBUG
	SGE_LOGI("OpenGL debug enabled.");
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &flags);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, flags | SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	sge_window_width = 800;
	sge_window_height = 600;
	sge_window_flags = SGE_WINDOW_VISIBLE | SGE_WINDOW_FOCUS;

	SGE_LOGD("Creating main window...");

	sge_window = SDL_CreateWindow("SGE",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		sge_window_width, sge_window_height,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (sge_window == NULL) {
		SGE_LOGE("Failed to create main window.");
		goto bad0;
	}
	
	sge_window_id = SDL_GetWindowID(sge_window);

	SGE_LOGD("Create OpenGL context...");
	sge_window_gl = SDL_GL_CreateContext(sge_window);
	if (sge_window_gl == NULL) {
		SGE_LOGE("Failed to create OpenGL context.");
		goto bad1;
	}

	SDL_GL_MakeCurrent(sge_window, sge_window_gl);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		goto bad2;

	SGE_LOGI("GLEW: %s", glewGetString(GLEW_VERSION));
	SGE_LOGI("OpenGL: %s", glGetString(GL_VERSION));
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

#if defined SGE_DEBUG && defined SGE_LOG
	if (GL_KHR_debug)
		glDebugMessageCallback(sge_window_gl_debug_output, NULL);
#endif

	SGE_LOGD("Creating GLEX context...");
	sge_window_glex = glexCreateContext();
	if (sge_window_glex == NULL) {
		SGE_LOGE("Failed to create GLEX context.");
		goto bad2;
	}
	
	glexMakeCurrent(sge_window_glex);

	SGE_LOGD("%s(%d)", __func__, __LINE__);
	glexRenderMode(GLEX_RENDER_MODE_FORWARD);
	SGE_LOGD("%s(%d)", __func__, __LINE__);

	SGE_LOGD("Creating nanovg context...");
	sge_window_nvg = nvgCreateGL3(0);
	if (sge_window_nvg == NULL) {
		SGE_LOGE("Failed to create nanovg context.");
		goto bad3;
	}

	return 0;

bad3:
	glexDeleteContext(sge_window_glex);
	sge_window_glex = NULL;

bad2:
	SDL_GL_DeleteContext(sge_window_gl);
	sge_window_gl = NULL;

bad1:
	SDL_DestroyWindow(sge_window);
	sge_window = NULL;

bad0:
	return -1;
}

void sge_window_shutdown(void)
{
	CX_ASSERT(sge_window != NULL);
	CX_ASSERT(sge_window_gl != NULL);
	CX_ASSERT(sge_window_glex != NULL);
	CX_ASSERT(sge_window_nvg != NULL);

	SGE_LOGD("Releasing nanovg context...");
	nvgDeleteGL3(sge_window_nvg);
	sge_window_nvg = NULL;

	SGE_LOGD("Releasing GLEX context...");
	glexDeleteContext(sge_window_glex);
	sge_window_glex = NULL;

	SGE_LOGD("Releasing OpenGL context...");
	SDL_GL_DeleteContext(sge_window_gl);
	sge_window_gl = NULL;

	SGE_LOGD("Releasing main window...");
	SDL_DestroyWindow(sge_window);
	sge_window = NULL;
}

void sge_window_handle_event(const SDL_WindowEvent *event)
{
	if (event->windowID != sge_window_id)
		return;

	switch (event->event) {
	case SDL_WINDOWEVENT_SHOWN:
		sge_window_flags |= SGE_WINDOW_VISIBLE;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		sge_window_flags &= ~SGE_WINDOW_VISIBLE;
		break;
	case SDL_WINDOWEVENT_RESIZED:
		sge_window_width = event->data1;
		sge_window_height = event->data2;
		break;
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		sge_window_flags |= SGE_WINDOW_FOCUS;
		break;
	case SDL_WINDOWEVENT_FOCUS_LOST:
		sge_window_flags &= ~SGE_WINDOW_FOCUS;
		break;
	}
}

void sge_window_update(float elapsed, const sge_window_drawer_t *drawer)
{
	if (!(sge_window_flags & SGE_WINDOW_VISIBLE))
		return;

	if (sge_window_width < 1 || sge_window_height < 1)
		return;

	if (SDL_GL_MakeCurrent(sge_window, sge_window_gl) < 0)
		return;

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, sge_window_width, sge_window_height);

	/* draw 3d */

	glexBeginFrame();

	if (drawer != NULL && drawer->draw_3d != NULL)
		drawer->draw_3d();

	glexEndFrame();

	/* draw 2d */

	nvgBeginFrame(sge_window_nvg, (float)sge_window_width, (float)sge_window_height, 1.0f);

	if (drawer != NULL && drawer->draw_2d != NULL)
		drawer->draw_2d(sge_window_nvg);

	nvgEndFrame(sge_window_nvg);

	SDL_GL_SwapWindow(sge_window);
}

