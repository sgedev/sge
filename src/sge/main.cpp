//
//
#include <string>
#include <iostream>

#include <argh.h>
#include <physfs.h>
#include <imgui.h>
#include <imgui_dialogs.h>
#include <filesystem/path.h>
#include <filesystem/resolver.h>
#include <GL/glew.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <sge/common.hpp>
#include <sge/game.hpp>

SGE_BEGIN

static SDL_Window *s_window;
static Uint32 s_window_id;
static int s_window_rect[4];
static bool s_window_visibled;
static SDL_GLContext s_gl_context;
static uv_loop_t *s_loop = NULL;
static uv_timer_t s_frame_timer;
static uint64_t s_last_time;

#ifdef SGE_DEBUG

static inline const char *gl_debug_source(GLenum source)
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

static inline const char *gl_debug_type(GLenum type)
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

static inline const char *gl_debug_severity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
	case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
	case GL_DEBUG_SEVERITY_LOW:          return "LOW";
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
	}
	return "";
}

static void gl_debug_output(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar *message, const void *data)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		SGE_LOGE("GL-%04d|%s|%s|%s] %s", id,
			gl_debug_source(source), gl_debug_type(type), gl_debug_severity(severity), message);
		break;
	default:
		SGE_LOGD("GL-%04d|%s|%s|%s] %s", id,
			gl_debug_source(source), gl_debug_type(type), gl_debug_severity(severity), message);
		break;
	}
}

#endif // SGE_DEBUG

static inline void handle_window_event(const SDL_Event &event)
{
	if (event.window.windowID != s_window_id)
		return;

	switch (event.window.event) {
	case SDL_WINDOWEVENT_MOVED:
		s_window_rect[0] = event.window.data1;
		s_window_rect[1] = event.window.data2;
		break;
	case SDL_WINDOWEVENT_RESIZED:
		s_window_rect[2] = event.window.data1;
		s_window_rect[3] = event.window.data2;
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

static inline void poll_events(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		handle_window_event(event);
		game::handle_event(event);
	}
}

static void frame(uv_timer_t *timer)
{
	int64_t pass = uv_now(s_loop) - s_last_time;
	if (pass < 0) {
		s_last_time = uv_now(s_loop);
		return;
	}

	float elapsed = float(pass) / 1000.0f;

	poll_events();

	game::update(elapsed);

	if (s_window_visibled && SDL_GL_MakeCurrent(s_window, s_gl_context) == 0) {
		glViewport(0, 0, s_window_rect[2], s_window_rect[3]);
		glClear(GL_COLOR_BUFFER_BIT);
		game::draw();
		SDL_GL_SwapWindow(s_window);
	}

	uv_update_time(s_loop);
	s_last_time = uv_now(s_loop);
}

static bool init(void)
{
	SGE_ASSERT(s_loop == NULL);
	s_loop = uv_default_loop();

	SGE_ASSERT(s_window == NULL);
	SGE_ASSERT(s_gl_context == NULL);

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
		return false;

	s_window_id = SDL_GetWindowID(s_window);
	SDL_GetWindowPosition(s_window, &s_window_rect[0], &s_window_rect[1]);
	SDL_GetWindowSize(s_window, &s_window_rect[2], &s_window_rect[3]);

	SGE_LOGD("Initializing OpenGL...\n");

	s_gl_context = SDL_GL_CreateContext(s_window);
	if (s_gl_context == NULL) {
		SGE_LOGE("Failed to create OpenGL context.\n");
		SDL_DestroyWindow(s_window);
		s_window = NULL;
		return false;
	}

	SDL_GL_MakeCurrent(s_window, s_gl_context);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		SGE_LOGE("Failed to initialize GLEW.\n");
		SDL_GL_DeleteContext(s_gl_context);
		s_gl_context = NULL;
		SDL_DestroyWindow(s_window);
		s_window = NULL;
		return false;
	}

#ifdef SGE_DEBUG
	if (GL_KHR_debug) {
		SGE_LOGD("Enable OpenGL debug output.\n");
		glDebugMessageCallback(gl_debug_output, NULL);
	}
#endif

	SGE_LOGI("GLEW: %s\n", glewGetString(GLEW_VERSION));
	SGE_LOGI("OpenGL: %s\n", glGetString(GL_VERSION));

	game::init();

	uv_timer_init(s_loop, &s_frame_timer);
	uv_timer_start(&s_frame_timer, frame, 0, 16);

	s_last_time = uv_now(s_loop);

	// SDL_SetRelativeMouseMode(SDL_FALSE);

	return true;
}

static void shutdown(void)
{
	SGE_LOGD("Shutdown...\n");

	uv_timer_stop(&s_frame_timer);

	game::shutdown();

	if (s_gl_context == SDL_GL_GetCurrentContext())
		SDL_GL_MakeCurrent(s_window, NULL);

	SDL_GL_DeleteContext(s_gl_context);
	s_gl_context = NULL;

	SDL_DestroyWindow(s_window);
	s_window = NULL;
	s_window_id = 0;

	SGE_LOGI("Shutdown.\n");
}

SGE_END

static void show_version(void)
{
	const char *type =
#ifdef SGE_DEBUG
		"DEBUG";
#else
		"RELEASE";
#endif

	SGE_LOGI("SGE: %d.%d.%d [%s]\n",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH, type);
}

static void show_help(const char *arg0)
{
	SGE_LOGI("%s [options] <game path>\n", arg0);
	SGE_LOGI("options:\n");
	SGE_LOGI("-h, --help\n");
	SGE_LOGI("    show this message.\n");
	SGE_LOGI("-v, --version\n");
	SGE_LOGI("    show version.\n");
}

int main(int argc, char *argv[])
{
	int ret = EXIT_FAILURE;
	argh::parser cmdline(argv);
	std::string game;
	filesystem::path path;

	SDL_SetMainReady();

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		goto end0;

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	if (!PHYSFS_init(NULL))
		goto end1;

	if (cmdline[{ "-v", "--version" }]) {
		show_version();
		goto end2;
	}

	if (cmdline[{ "-h", "--help" }]) {
		show_help(argv[0]);
		goto end2;
	}

	show_version();

	SGE_LOGD("Platform: %s, CPUs %d, Memory %dMB\n",
		SDL_GetPlatform(), SDL_GetCPUCount(), SDL_GetSystemRAM());

	if (cmdline.size() < 2) {
		SGE_LOGE("No game path.\n");
		goto end2;
	}

	if (cmdline.size() > 2) {
		SGE_LOGE("Too many game path.\n");
		goto end0;
	}

	cmdline(1) >> game;
	path = game;
	path = path.make_absolute();
	if (!path.exists()) {
		SGE_LOGE("Invalid game path.\n");
		goto end2;
	}

	SGE_LOGI("Game path: '%s'\n", path.str().c_str());
	if (!PHYSFS_mount(path.str().c_str(), "/", 0)) {
		SGE_LOGE("failed to mount root (%d).\n", PHYSFS_getLastErrorCode());
		goto end2;
	}

	if (!sge::init())
		goto end2;

	SGE_LOGD("Running...\n");
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	sge::shutdown();
	ret = EXIT_SUCCESS;

end2:
	PHYSFS_deinit();

end1:
	SDL_Quit();

end0:
	SGE_LOGD("Quit.\n");

	return ret;
}

