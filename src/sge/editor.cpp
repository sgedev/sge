//
//
#include <sge/editor.hpp>

SGE_BEGIN

editor::editor(uv_loop_t *loop, const filesystem::path &path)
	: game(loop, path)
{
}

editor::~editor(void)
{
}

bool editor::init(void)
{
	if (!game::init())
		return false;

	main_window().set_title("SGE Editor");

	return true;
}

void editor::shutdown(void)
{
	game::shutdown();
}

void editor::handle_event(const SDL_Event &event)
{
	game::handle_event(event);
}

void editor::update(float elapsed)
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

SGE_END

