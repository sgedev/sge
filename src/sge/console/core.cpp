//
//
#include <string>

#include <imgui.h>
#include <imgui_dialogs.h>

#include <sge/console.hpp>

SGE_CONSOLE_BEGIN

static std::string s_path;
static bool s_dirty;

static void on_game_save(void);

static void on_game_new(void)
{
	if (s_dirty) {
	}
}

static void on_game_open(void)
{
}

static void on_game_save(void)
{
	if (!s_dirty)
		return;
}

static void on_game_save_as(void)
{
}

static void on_game_close(void)
{
	if (s_dirty)
		on_game_save();

	if (!s_dirty)
		return;
}

static void on_game_import(void)
{

}

static void on_game_export(void)
{

}

static void on_game_quit(void)
{
	uv_stop(uv_default_loop());
}

static void draw_game_menu(void)
{
	if (ImGui::BeginMenu("Game")) {
		if (ImGui::MenuItem("New..."))
			on_game_new();
		if (ImGui::MenuItem("Open..."))
			on_game_open();
		if (ImGui::MenuItem("Save"))
			on_game_save();
		if (ImGui::MenuItem("Save As..."))
			on_game_save_as();
		if (ImGui::MenuItem("Close"))
			on_game_close();
		ImGui::Separator();
		if (ImGui::MenuItem("Import..."))
			on_game_import();
		if (ImGui::MenuItem("Export..."))
			on_game_export();
		ImGui::Separator();
		if (ImGui::MenuItem("Quit"))
			on_game_quit();
		ImGui::EndMenu();
	}
}

static void on_edit_cut(void)
{
}

static void on_edit_copy(void)
{
}

static void draw_edit_menu(void)
{
	if (ImGui::BeginMenu("Edit")) {
		if (ImGui::MenuItem("Cut"))
			on_edit_cut();
		if (ImGui::MenuItem("Copy"))
			on_edit_copy();
		ImGui::EndMenu();
	}
}

static void on_view_scene(void)
{
}

static void draw_view_menu(void)
{
	if (ImGui::BeginMenu("View")) {
		if (ImGui::MenuItem("Scene"))
			on_view_scene();
		ImGui::EndMenu();
	}
}

static void on_tools_options(void)
{
}

static void draw_tools_menu(void)
{
	if (ImGui::BeginMenu("Tools")) {
		if (ImGui::MenuItem("Options..."))
			on_tools_options();
		ImGui::EndMenu();
	}
}

static void on_help_about(void)
{
}

static void draw_help_menu(void)
{
	if (ImGui::BeginMenu("Help")) {
		ImGui::Separator();
		if (ImGui::MenuItem("About..."))
			on_help_about();
		ImGui::EndMenu();
	}
}

bool init(void)
{
	return true;
}

void shutdown(void)
{
}

bool handle_event(const SDL_Event &event)
{
	if (event.type == SDL_QUIT) {
		on_game_quit();
		return true;
	}

	return false;
}

void update(float elapsed)
{
}

void draw(void)
{
	if (ImGui::BeginMainMenuBar()) {
		draw_game_menu();
		draw_edit_menu();
		draw_view_menu();
		draw_tools_menu();
		draw_help_menu();
		ImGui::EndMainMenuBar();
	}
}

SGE_CONSOLE_END

