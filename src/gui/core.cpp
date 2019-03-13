//
//
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include <sge/gl.h>
#include <sge/gui.h>

SGE_GUI_BEGIN

static SDL_Window *window;

bool init(void)
{
	SGE_LOGI("Initializing GUI...\n");

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	window = gl::get_window();

	ImGui_ImplSDL2_InitForOpenGL(window, gl::get_context());
	ImGui_ImplOpenGL3_Init();

	return true;
}

void shutdown(void)
{
	SGE_ASSERT(window != NULL);

	SGE_LOGI("Shuting down GUI...\n");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
}

void update(void)
{

}

void draw(void)
{
	SGE_ASSERT(window != NULL);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
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
	ImGui::Text("fps %d", fps());
	ImGui::End();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void handle_event(const SDL_Event *event)
{
	SGE_ASSERT(window != NULL);

	ImGui_ImplSDL2_ProcessEvent(event);
}

SGE_GUI_END
