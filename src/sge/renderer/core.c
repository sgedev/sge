/*
 *
 */
#include <SDL.h>
#include <GL/gl3w.h>

#include <nanovg.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

#include <sge/object.h>
#include <sge/renderer.h>

#define SGE_RENDERER_WINDOW_VISIBLE 0x1
#define SGE_RENDERER_WINDOW_FOCUS 0x2

static SDL_Window *sge_renderer_window;
static Uint32 sge_renderer_window_id;
static int sge_renderer_window_flags;
static int sge_renderer_window_rect[4];
static SDL_GLContext *sge_renderer_gl;
static NVGcontext *sge_renderer_nvg;

static void sge_renderer_object_added(sge_object_t *object)
{
}

static void sge_renderer_object_removed(sge_object_t *object)
{
}

static sge_object_watch_t sge_renderer_object_watch = {
	.object_added = sge_renderer_object_added,
	.object_removed = sge_renderer_object_removed,
};

static void sge_renderer_handle_window_event(const SDL_WindowEvent *evt)
{
	if (evt->windowID != sge_renderer_window_id)
		return;

	switch (evt->event) {
	case SDL_WINDOWEVENT_SHOWN:
		sge_renderer_window_flags |= SGE_RENDERER_WINDOW_VISIBLE;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		sge_renderer_window_flags &= ~SGE_RENDERER_WINDOW_VISIBLE;
		break;
	case SDL_WINDOWEVENT_MOVED:
		sge_renderer_window_rect[0] = evt->data1;
		sge_renderer_window_rect[1] = evt->data2;
		break;
	case SDL_WINDOWEVENT_RESIZED:
		sge_renderer_window_rect[2] = evt->data1;
		sge_renderer_window_rect[3] = evt->data2;
		break;
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		sge_renderer_window_flags |= SGE_RENDERER_WINDOW_FOCUS;
		break;
	case SDL_WINDOWEVENT_FOCUS_LOST:
		sge_renderer_window_flags &= ~SGE_RENDERER_WINDOW_FOCUS;
		break;
	default:
		break;
	}
}

void sge_renderer_export(lua_State *L)
{
	lua_newtable(L);

	lua_setglobal(L, "renderer");
}

bool sge_renderer_init(void)
{
#ifdef SGE_DEBUG
	int gl_flags;
#endif

	SGE_ASSERT(sge_renderer_window == NULL);
	SGE_ASSERT(sge_renderer_gl == NULL);
	SGE_ASSERT(sge_renderer_nvg == NULL);

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

	sge_renderer_window = SDL_CreateWindow("SGE",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (sge_renderer_window == NULL) {
		SGE_LOGE("Failed to create main window.\n");
		goto bad0;
	}

	sge_renderer_window_id = SDL_GetWindowID(sge_renderer_window);
	sge_renderer_window_flags = SGE_RENDERER_WINDOW_FOCUS | SGE_RENDERER_WINDOW_VISIBLE;

	SDL_GetWindowPosition(sge_renderer_window,
		&sge_renderer_window_rect[0], &sge_renderer_window_rect[1]);

	SDL_GetWindowSize(sge_renderer_window,
		&sge_renderer_window_rect[2], &sge_renderer_window_rect[3]);

	SGE_LOGD("Create OpenGL context...\n");
	sge_renderer_gl = SDL_GL_CreateContext(sge_renderer_window);
	if (sge_renderer_gl == NULL) {
		SGE_LOGE("Failed to create OpenGL context.\n");
		goto bad1;
	}

	SDL_GL_MakeCurrent(sge_renderer_window, sge_renderer_gl);

	if (gl3wInit2((GL3WGetProcAddressProc)SDL_GL_GetProcAddress) < 0) {
		SGE_LOGE("Failed to init OpenGL procs.\n");
		goto bad2;
	}

	SGE_LOGI("OpenGL: %s\n", glGetString(GL_VERSION));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

#if defined SGE_DEBUG
	//if (GL_KHR_debug)
	//	glDebugMessageCallback(glDebugOutput, this);
#endif

	sge_renderer_nvg = nvgCreateGL3(0);
	if (sge_renderer_nvg == NULL) {
		SGE_LOGE("Failed to init NANOVG.\n");
		goto bad2;
	}

	sge_object_watch_add(&sge_renderer_object_watch);

	return true;

bad2:
	SDL_GL_DeleteContext(sge_renderer_gl);
	sge_renderer_gl = NULL;

bad1:
	SDL_DestroyWindow(sge_renderer_window);
	sge_renderer_window = NULL;

bad0:
	return false;
}

void sge_renderer_shutdown(void)
{
	SGE_ASSERT(sge_renderer_window != NULL);
	SGE_ASSERT(sge_renderer_gl != NULL);
	SGE_ASSERT(sge_renderer_nvg != NULL);

	sge_object_watch_remove(&sge_renderer_object_watch);

	nvgDeleteGL3(sge_renderer_nvg);
	sge_renderer_nvg = NULL;

	SDL_GL_DeleteContext(sge_renderer_gl);
	sge_renderer_gl = NULL;

	SDL_DestroyWindow(sge_renderer_window);
	sge_renderer_window = NULL;
}

void sge_renderer_handle_event(const SDL_Event *evt)
{
	switch (evt->type) {
	case SDL_WINDOWEVENT:
		sge_renderer_handle_window_event(&evt->window);
		break;
	default:
		break;
	}
}

void sge_renderer_draw(float elapsed)
{
	if (!(sge_renderer_window_flags & SGE_RENDERER_WINDOW_VISIBLE))
		return;

	if (sge_renderer_window_rect[2] < 1 || sge_renderer_window_rect[3] < 1)
		return;

	SGE_ASSERT(sge_renderer_window != NULL);
	SGE_ASSERT(sge_renderer_gl != NULL);
	SGE_ASSERT(sge_renderer_nvg != NULL);

	if (SDL_GL_MakeCurrent(sge_renderer_window, sge_renderer_gl) < 0)
		return;

	glViewport(0, 0, sge_renderer_window_rect[2], sge_renderer_window_rect[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	/* TOOD draw 3d */

	nvgBeginFrame(sge_renderer_nvg,
		(float)sge_renderer_window_rect[2], (float)sge_renderer_window_rect[3], 1.0f);

	/* TODO draw 2d */
	nvgBeginPath(sge_renderer_nvg);
	nvgRect(sge_renderer_nvg, 100, 100, 120, 30);
	nvgFillColor(sge_renderer_nvg, nvgRGBA(255, 192, 0, 255));
	nvgFill(sge_renderer_nvg);

	nvgEndFrame(sge_renderer_nvg);

	SDL_GL_SwapWindow(sge_renderer_window);
}
