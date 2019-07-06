//
//
#include <sge/gui.hpp>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

SGE_GUI_BEGIN

static SDL_Window *s_window;
static window *s_test_window;

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

bool init(void)
{
	SDL_Window *gl_window = SDL_GL_GetCurrentWindow();
	SDL_GLContext gl_context = SDL_GL_GetCurrentContext();

	if (gl_window == NULL || gl_context == NULL)
		return false;

	IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
	io.IniFilename = NULL;

	SGE_LOGI("Dear ImGui: %s\n", ImGui::GetVersion());

    ImGui::StyleColorsDark();

	ImGuiStyle &style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 0.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 0.0f;

    ImGui_ImplSDL2_InitForOpenGL(gl_window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");

	s_window = gl_window;

	s_test_window = new window;

	return true;
}

void shutdown(void)
{
	delete s_test_window;

	SGE_ASSERT(s_window != NULL);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
}

bool handle_event(const SDL_Event &event)
{
	SGE_ASSERT(s_window != NULL);

	return ImGui_ImplSDL2_ProcessEvent(&event);
}

void update(float elapsed)
{
	SGE_ASSERT(s_window != NULL);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(s_window);
	ImGui::NewFrame();

	widget::update_all(elapsed);

	ImGui::ShowDemoWindow(NULL);
	show_fps();

	ImGui::Render();
}

void draw(void)
{
	SGE_ASSERT(s_window != NULL);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

SGE_GUI_END

