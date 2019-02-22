//
//
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include <sge/gui.hpp>

SGE_GUI_BEGIN

static SDL_Window *window;
static bool show_demo;

bool init(void)
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	window = SDL_GL_GetCurrentWindow();

	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();

	show_demo = true;

	return true;
}

void shutdown(void)
{
	SGE_ASSERT(window != NULL);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
}

void draw(void)
{
	SGE_ASSERT(window != NULL);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    if (show_demo)
        ImGui::ShowDemoWindow(&show_demo);

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void handle_event(const SDL_Event *event)
{
	SGE_ASSERT(window != NULL);

	ImGui_ImplSDL2_ProcessEvent(event);
}

SGE_GUI_END
