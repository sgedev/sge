//
//
#include <future>

#include <lua.hpp>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <GL/glew.h>

#include <sge/scene.hpp>
#include <sge/game.hpp>

#include "input.hpp"
#include "vm.hpp"

SGE_GAME_BEGIN

static SDL_Window *s_window;
static Uint32 s_window_id;
static glm::ivec4 s_window_rect;
static bool s_window_visibled;
static SDL_GLContext s_gl_context;
static vm s_init;
static bool s_running;
static bool s_fullscreen;

static inline bool handle_hardcode_event(const SDL_Event &event)
{
	if (event.type == SDL_WINDOWEVENT && event.window.windowID == s_window_id) {
		switch (event.window.event) {
		case SDL_WINDOWEVENT_MOVED:
			s_window_rect.x = event.window.data1;
			s_window_rect.y = event.window.data2;
			break;
		case SDL_WINDOWEVENT_RESIZED:
			s_window_rect.z = event.window.data1;
			s_window_rect.w = event.window.data2;
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

	if (event.type == SDL_KEYDOWN && event.window.windowID == s_window_id) {
		if (event.key.keysym.sym == SDLK_RETURN && event.key.keysym.mod & KMOD_ALT) {
			s_fullscreen = !s_fullscreen;
			if (s_fullscreen)
				SDL_SetWindowFullscreen(s_window, SDL_WINDOW_FULLSCREEN);
			else
				SDL_SetWindowFullscreen(s_window, 0);
		}
	}

	return true;
}

static inline void show_fps(void)
{
	static int i = 0;
	static double fps = 0;

	if ((++i) > 4) {
		fps = ImGui::GetIO().Framerate;
		i = 0;
	}
		
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("fps", NULL, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);

	ImVec4 fps_color;

	if (fps > 59)
		fps_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	else if (fps < 24)
		fps_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	else
		fps_color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

	ImGui::TextColored(fps_color, "fps %.2f", fps);
	ImGui::End();
}

static bool init_dear_imgui(void)
{
	IMGUI_CHECKVERSION();

    ImGui::CreateContext();
	SGE_LOGI("Dear ImGui: %s\n", ImGui::GetVersion());

    ImGuiIO &io = ImGui::GetIO();
	io.IniFilename = NULL;

    ImGui::StyleColorsDark();

	ImGuiStyle &style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 0.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 0.0f;

    ImGui_ImplSDL2_InitForOpenGL(s_window, s_gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");

	return true;
}

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
	return "UNKNOWN_SOURCE";
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
	return "UNKNOWN_TYPE";
}

static inline const char *gl_debug_severity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
	case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
	case GL_DEBUG_SEVERITY_LOW:          return "LOW";
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
	}
	return "UNKNOWN_SEVERITY";
}

static void gl_debug_output(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar *message, const void *data)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		SGE_LOGE("GL-%04d|%s|%s|%s] %s", id,
			gl_debug_source(source), gl_debug_type(type), gl_debug_severity(severity), message);
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
	case GL_DEBUG_TYPE_PORTABILITY:
	case GL_DEBUG_TYPE_PERFORMANCE:
		SGE_LOGW("GL-%04d|%s|%s|%s] %s", id,
			gl_debug_source(source), gl_debug_type(type), gl_debug_severity(severity), message);
		break;
	default:
		SGE_LOGD("GL-%04d|%s|%s|%s] %s", id,
			gl_debug_source(source), gl_debug_type(type), gl_debug_severity(severity), message);
		break;
	}
}

#endif // SGE_DEBUG

bool init(void)
{
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
		goto bad0;

	s_fullscreen = false;
	s_window_id = SDL_GetWindowID(s_window);
	SDL_GetWindowPosition(s_window, &s_window_rect[0], &s_window_rect[1]);
	SDL_GetWindowSize(s_window, &s_window_rect[2], &s_window_rect[3]);

	SGE_LOGD("Initializing OpenGL...\n");

	s_gl_context = SDL_GL_CreateContext(s_window);
	if (s_gl_context == NULL) {
		SGE_LOGE("Failed to create OpenGL context.\n");
		goto bad1;
	}

	SDL_GL_MakeCurrent(s_window, s_gl_context);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		SGE_LOGE("Failed to initialize GLEW.\n");
		goto bad2;
	}

	SGE_LOGI("GLEW: %s\n", glewGetString(GLEW_VERSION));
	SGE_LOGI("OpenGL: %s\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

#ifdef SGE_DEBUG
	if (GL_KHR_debug) {
		SGE_LOGD("Enable OpenGL debug output.\n");
		glDebugMessageCallback(gl_debug_output, NULL);
	}
#endif

	scene::init();
	input::init();
	init_dear_imgui();

	s_init.start("/init.lua");
	s_running = true;

	return true;

bad2:
	SDL_GL_DeleteContext(s_gl_context);
	s_gl_context = NULL;

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
	SGE_ASSERT(s_gl_context != NULL);

	s_init.stop();

	input::shutdown();
	scene::shutdown();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	if (s_gl_context == SDL_GL_GetCurrentContext())
		SDL_GL_MakeCurrent(s_window, NULL);

	SDL_GL_DeleteContext(s_gl_context);
	s_gl_context = NULL;

	SDL_DestroyWindow(s_window);
	s_window = NULL;
	s_window_id = 0;
}

bool handle_event(const SDL_Event &event)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_gl_context != NULL);

	if (event.type == SDL_QUIT) {
		s_running = false;
		return true;
	}

	handle_hardcode_event(event);

	ImGui_ImplSDL2_ProcessEvent(&event);
	input::handle_event(event);
}

void frame(float elapsed)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_gl_context != NULL);

	if (!s_running)
		return;

	input::update(elapsed);

	// TODO game logic update.

	scene::update(elapsed);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(s_window);
	ImGui::NewFrame();

	// TODO gui update

	ImGui::ShowDemoWindow(NULL);
	show_fps();

	ImGui::Render();

	if (s_window_visibled && SDL_GL_MakeCurrent(s_window, s_gl_context) == 0) {
		glViewport(0, 0, s_window_rect[2], s_window_rect[3]);
		glClear(GL_COLOR_BUFFER_BIT);
		scene::draw();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(s_window);
	}
}

bool is_running(void)
{
	return s_running;
}

SGE_GAME_END

