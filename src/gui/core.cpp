//
//
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "SGL_gui.h"

SGL_GUI_BEGIN

static SDL_Window *Window;
static bool ShowDemo;

bool Init(void)
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	Window = SDL_GL_GetCurrentWindow();

	ImGui_ImplSDL2_InitForOpenGL(Window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();

	ShowDemo = true;

	return true;
}

void Shutdown(void)
{
	SGL_ASSERT(Window != NULL);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
}

void Draw(float elapsed)
{
	SGL_ASSERT(Window != NULL);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(Window);
    ImGui::NewFrame();

    if (ShowDemo)
        ImGui::ShowDemoWindow(&ShowDemo);

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void HandleEvent(const SDL_Event *event)
{
	SGL_ASSERT(Window != NULL);

	ImGui_ImplSDL2_ProcessEvent(event);
}

SGL_GUI_END
