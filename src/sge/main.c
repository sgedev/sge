//
//
#include <stdio.h>
#include <stdlib.h>

#include <parg.h>
#include <nanovg.h>
#include <GL/glew.h>
#include <GL/glex.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <sge/common.h>
#include <sge/vm.h>

#define SGE_WINDOW_VISIBLE 0x1
#define SGE_WINDOW_FOCUS 0x2

static SDL_Window *sge_window;
static Uint32 sge_window_id;
static int sge_window_width;
static int sge_window_height;
static int sge_window_flags;
static SDL_GLContext sge_gl;
static GLEXContext *sge_glex;
static NVGcontext *sge_nvg;
static int sge_run;
static int sge_fps;
static int sge_fps_count;
static Uint32 sge_fps_last;
static bool sge_show_fps;
static Uint32 sge_elapsed_min;
static Uint32 sge_last;

#if defined SGE_DEBUG && defined SGE_LOG

static const char *sge_gl_debug_source(GLenum source)
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

static const char *sge_gl_debug_type(GLenum type)
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

static const char *sge_gl_debug_serverity(GLenum severity)
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

static void sge_gl_debug_output(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar *message, const void *data)
{
	if (type == GL_DEBUG_TYPE_ERROR) {
		SGE_LOGE("GL|%s|%s|%s: %s\n",
			sge_gl_debug_serverity(severity),
			sge_gl_debug_source(source),
			sge_gl_debug_type(type), message);
	} else {
		SGE_LOGD("GL|%s|%s|%s: %s\n",
			sge_gl_debug_serverity(severity),
			sge_gl_debug_source(source),
			sge_gl_debug_type(type), message);
	}
}

#endif /* SGE_DEBUG && SGE_LOG */

static void sge_handle_window_event(const SDL_WindowEvent *event)
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

static void sge_poll_events(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			sge_run = 0;
			break;
		case SDL_WINDOWEVENT:
			sge_handle_window_event(&event.window);
			break;
		default:
			sge_vm_handle_event(&event);
			break;
		}
	}
}

static bool sge_init(const char *vm_root)
{
	int flags;

	CX_ASSERT(sge_window == NULL);
	CX_ASSERT(sge_gl == NULL);
	CX_ASSERT(sge_glex == NULL);
	CX_ASSERT(sge_nvg == NULL);

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
	sge_gl = SDL_GL_CreateContext(sge_window);
	if (sge_gl == NULL) {
		SGE_LOGE("Failed to create OpenGL context.");
		goto bad1;
	}

	SDL_GL_MakeCurrent(sge_window, sge_gl);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		goto bad2;

	SGE_LOGI("GLEW: %s", glewGetString(GLEW_VERSION));
	SGE_LOGI("OpenGL: %s", glGetString(GL_VERSION));
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

#if defined SGE_DEBUG && defined SGE_LOG
	if (GL_KHR_debug)
		glDebugMessageCallback(sge_gl_debug_output, NULL);
#endif

	SGE_LOGD("Creating GLEX context...");
	sge_glex = glexCreateContext();
	if (sge_glex == NULL) {
		SGE_LOGE("Failed to create GLEX context.");
		goto bad2;
	}
	
	glexMakeCurrent(sge_glex);
	glexRenderMode(GLEX_RENDER_MODE_FORWARD);

	SGE_LOGD("Creating nanovg context...");
	sge_nvg = nvgCreateGL3(0);
	if (sge_nvg == NULL) {
		SGE_LOGE("Failed to create nanovg context.");
		goto bad3;
	}

	/* for test */
	nvgCreateFont(sge_nvg, "default", "C:\\Users\\Shaomy\\Documents\\GitHub\\sge\\build\\Roboto-Light.ttf");

	if (!sge_vm_init(vm_root))
		goto bad4;

	return true;

bad4:
	nvgDeleteGL3(sge_nvg);
	sge_nvg = NULL;

bad3:
	glexDeleteContext(sge_glex);
	sge_glex = NULL;

bad2:
	SDL_GL_DeleteContext(sge_gl);
	sge_gl = NULL;

bad1:
	SDL_DestroyWindow(sge_window);
	sge_window = NULL;

bad0:
	return false;
}

static void sge_shutdown(void)
{
	CX_ASSERT(sge_window != NULL);
	CX_ASSERT(sge_gl != NULL);
	CX_ASSERT(sge_glex != NULL);
	CX_ASSERT(sge_nvg != NULL);

	sge_vm_shutdown();

	SGE_LOGD("Releasing nanovg context...");
	nvgDeleteGL3(sge_nvg);
	sge_nvg = NULL;

	SGE_LOGD("Releasing GLEX context...");
	glexDeleteContext(sge_glex);
	sge_glex = NULL;

	SGE_LOGD("Releasing OpenGL context...");
	SDL_GL_DeleteContext(sge_gl);
	sge_gl = NULL;

	SGE_LOGD("Releasing main window...");
	SDL_DestroyWindow(sge_window);
	sge_window = NULL;
}


static void sge_vm_trap_exit(void)
{
	sge_run = 0;
}

static int sge_vm_trap_get_fps(void)
{
	return sge_fps;
}

static int sge_vm_trap_get_fps_max(void)
{
	SGE_ASSERT(sge_elapsed_min > 0);
	return 1000 / sge_elapsed_min;
}

static void sge_vm_trap_set_fps_max(int v)
{
	SGE_ASSERT(v > 0);
	sge_elapsed_min = 1000 / v;
}

static void sge_vm_trap_toggle_show_fps(void)
{
	sge_show_fps = !sge_show_fps;
}

static void sge_frame(float elapsed)
{
	static const sge_vm_traps_t traps = {
		.exit = sge_vm_trap_exit,
		.get_fps = sge_vm_trap_get_fps,
		.get_fps_max = sge_vm_trap_get_fps_max,
		.set_fps_max = sge_vm_trap_set_fps_max,
		.toggle_show_fps = sge_vm_trap_toggle_show_fps
	};

	sge_vm_update(elapsed, &traps);

	if (!(sge_window_flags & SGE_WINDOW_VISIBLE))
		return;

	if (sge_window_width < 1 || sge_window_height < 1)
		return;

	if (SDL_GL_MakeCurrent(sge_window, sge_gl) < 0)
		return;

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, sge_window_width, sge_window_height);

	glexBeginFrame();

	sge_vm_draw_glex();

	glexEndFrame();

	nvgBeginFrame(sge_nvg, (float)sge_window_width, (float)sge_window_height, 1.0f);

	nvgFontFace(sge_nvg, "default");
	nvgFontSize(sge_nvg, 20.0f);

	sge_vm_draw_nvg(sge_nvg);

	if (sge_show_fps) {
		char buf[16];
		sprintf(buf, "fps: %d", sge_fps);
		nvgFillColor(sge_nvg, nvgRGBA(255, 255, 255, 255));
		nvgTextAlign(sge_nvg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
		nvgText(sge_nvg, 0, 0, buf, NULL);
	}

	nvgEndFrame(sge_nvg);

	SDL_GL_SwapWindow(sge_window);
}

static void sge_print_help(void)
{
	SGE_LOGI("Usage:");
	SGE_LOGI("  sge [options] <root>");
	SGE_LOGI("Options:");
	SGE_LOGI("  -h Show this message.");
	SGE_LOGI("  -v Show version message.");
}

static void sge_print_version(void)
{
	SGE_LOGI("%d.%d.%d",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);
}

static void sge_log_output(void *data, int category, SDL_LogPriority priority, const char *message)
{
	if (priority == SDL_LOG_PRIORITY_ERROR || priority == SDL_LOG_PRIORITY_CRITICAL)
		fprintf(stderr, "SGE: %s\n", message);
	else
		fprintf(stdout, "SGE: %s\n", message);
}

int main(int argc, char *argv[])
{
	int ret;
	Uint32 curr;
	Uint32 pass;
	Uint32 delay;
	struct parg_state ps;
	char opt;
	const char *vm_root = NULL;

	SDL_LogSetOutputFunction(sge_log_output, NULL);
#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	SGE_LOGI("Simple Game Engine - %d.%d.%d\n",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);

	parg_init(&ps);

	while ((opt = parg_getopt(&ps, argc, argv, "hv")) != -1) {
		switch (opt) {
		case 1:
			vm_root = ps.optarg;
			break;
		case 'h':
			sge_print_help();
			return EXIT_SUCCESS;
		case 'v':
			sge_print_version();
			return EXIT_SUCCESS;
		default:
			SGE_LOGE("Unknown option -%c\n", opt);
			return EXIT_FAILURE;
		}
	}

	if (vm_root == NULL) {
		SGE_LOGE("VM root is not set.");
		return EXIT_FAILURE;
	}

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0) {
		SGE_LOGI("Failed to init SDL.");
		return EXIT_FAILURE;
	}

	ret = sge_init(vm_root);
	if (ret < 0) {
		SDL_Quit();
		SGE_LOGE("Failed to init SGE.");
		return EXIT_FAILURE;
	}

	sge_last = SDL_GetTicks();
	sge_fps_last = sge_last;
	sge_fps = 0;
	sge_fps_count = 0;
	sge_elapsed_min = 1000 / 60;
	sge_run = 1;

	while (sge_run) {
		sge_poll_events();

		curr = SDL_GetTicks();
		if (curr < sge_last) {
			sge_last = curr;
			continue;
		}

		pass = curr - sge_last;
		if (pass < sge_elapsed_min) {
			delay = sge_elapsed_min - pass;
			SDL_Delay(SDL_min(delay, 100));
			continue;
		}

		sge_frame(((float)pass) / 1000.0f);

		pass = curr - sge_fps_last;
		if (pass >= 1000) {
			sge_fps = sge_fps_count;
			sge_fps_count = 0;
			sge_fps_last = curr;
		} else
			sge_fps_count += 1;

		sge_last = curr;
	}

	sge_shutdown();
	SDL_Quit();

	return 0;
}

