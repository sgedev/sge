//
//
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include <sge/gui.h>

SGE_GUI_BEGIN

static SDL_Window *Window;
static bool Demo;

bool Init(void)
{
	SGE_LOGI("Initializing GUI...\n");

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	Window = SDL_GL_GetCurrentWindow();

	ImGui_ImplSDL2_InitForOpenGL(Window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();

	Demo = true;

	return true;
}

void Shutdown(void)
{
	SGE_ASSERT(Window != NULL);

	SGE_LOGI("Shuting down GUI...\n");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
}

void Update(void)
{

}

void Draw(void)
{
	SGE_ASSERT(Window != NULL);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(Window);
    ImGui::NewFrame();

	bool v = true;
	ImGui::Begin("Debug", &v,
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(100, 50));
	ImGui::Text("fps %d", GetFPS());
	ImGui::End();

    if (Demo)
        ImGui::ShowDemoWindow(&Demo);

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void HandleEvent(const SDL_Event *event)
{
	SGE_ASSERT(Window != NULL);

	ImGui_ImplSDL2_ProcessEvent(event);
}

SGE_GUI_END
