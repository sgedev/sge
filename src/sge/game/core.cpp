//
//
#include <future>

#include <lua.hpp>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <GL/glew.h>

#include <sge/gl.hpp>
#include <sge/scene.hpp>
#include <sge/game.hpp>

#include "input.hpp"
#include "vm.hpp"

SGE_GAME_BEGIN

static vm s_init;
static bool s_running;
static ImGuiContext *s_imgui;

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

	// test
	ImGui::Begin("main", NULL, ImGuiWindowFlags_NoDecoration);
	ImGui::Button("New Game");
	ImGui::Button("Quit");
	ImGui::End();
}

static bool init_dear_imgui(void)
{
	IMGUI_CHECKVERSION();

    s_imgui = ImGui::CreateContext();
	if (s_imgui == NULL) {
		SGE_LOGE("Failed to create imgui context.\n");
		return false;
	}

	ImGui::SetCurrentContext(s_imgui);

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

    ImGui_ImplSDL2_InitForOpenGL(gl::window(), gl::context());
    ImGui_ImplOpenGL3_Init("#version 130");

	return true;
}

static void shutdown_dear_imgui(void)
{
	SGE_ASSERT(s_imgui != NULL);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	ImGui::SetCurrentContext(NULL);
	ImGui::DestroyContext(s_imgui);
	s_imgui = NULL;
}

static inline bool handle_event(const SDL_Event &event)
{
	SGE_ASSERT(s_imgui != NULL);

	if (event.type == SDL_QUIT) {
		s_running = false;
		return true;
	}

	gl::handle_event(event);
	input::handle_event(event);

	ImGui::SetCurrentContext(s_imgui);
	ImGui_ImplSDL2_ProcessEvent(&event);
}

static inline void frame(float elapsed)
{
	SGE_ASSERT(s_imgui != NULL);

	input::update(elapsed);

	// TODO game logic update.

	scene::update(elapsed);

	ImGui::SetCurrentContext(s_imgui);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(gl::window());
	ImGui::NewFrame();

	// TODO gui update

	ImGui::ShowDemoWindow(NULL);
	show_fps();

	ImGui::Render();

	if (gl::make_current()) {
		scene::render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		gl::swap_buffers();
	}
}

int main(void)
{
	Uint32 last;
	Uint32 pass;

	gl::init();
	scene::init();
	input::init();

	init_dear_imgui();

	s_init.start("/init.lua");
	s_running = true;

	last = SDL_GetTicks();

	while (s_running) {
		pass = SDL_GetTicks() - last;
		if (pass < 0) {
			last = SDL_GetTicks();
			continue;
		}

		SDL_Event event;

		while (SDL_PollEvent(&event))
			handle_event(event);

		if (pass < 16) {
			SDL_Delay(16 - pass);
			continue;
		}

		frame(float(pass) / 1000.0f);
		last = SDL_GetTicks();
	}

	s_init.stop();

	shutdown_dear_imgui();

	input::shutdown();
	scene::shutdown();
	gl::shutdown();

	return EXIT_SUCCESS;
}

SGE_GAME_END

