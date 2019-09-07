//
//
#include <imgui.h>

#include <sge/window.hpp>
#include <sge/editor.hpp>

SGE_EDITOR_BEGIN

bool init(void)
{
	window::set_title("SGE Editor");

	return true;
}

void shutdown(void)
{
}

void handle_event(const SDL_Event *event)
{
}

void update(float elapsed)
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("New...");
			ImGui::MenuItem("Open...");
			ImGui::MenuItem("Save...");
			ImGui::MenuItem("Save As...");
			ImGui::Separator();
			ImGui::MenuItem("Quit");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			ImGui::MenuItem("Undo");
			ImGui::MenuItem("Redo");
			ImGui::MenuItem("Cut");
			ImGui::MenuItem("Copy");
			ImGui::MenuItem("Paste");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			ImGui::MenuItem("Develop view");
			ImGui::MenuItem("Console");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Game")) {
			ImGui::MenuItem("Reset");
			ImGui::MenuItem("Clear");
			ImGui::MenuItem("Load...");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tool")) {
			ImGui::MenuItem("Options...");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			ImGui::MenuItem("About...");
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void draw(void)
{

}

SGE_EDITOR_END
