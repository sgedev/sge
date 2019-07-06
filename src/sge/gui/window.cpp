//
//
#include <imgui.h>

#include <sge/gui/window.hpp>

SGE_GUI_BEGIN

window::window(void)
{
}

window::~window(void)
{
}

void window::update(float elapsed)
{
	const glm::vec2 &p = pos();
	const glm::vec2 &s = size();

	ImGui::SetNextWindowPos(ImVec2(p.x, p.y), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(s.x, s.y), ImGuiCond_Once);

	ImGui::Begin(name());

	widget::update(elapsed);

	ImVec2 p2 = ImGui::GetWindowPos();
	ImVec2 s2 = ImGui::GetWindowSize();

	move(p2.x, p2.y);
	resize(s2.x, s2.y);

	ImGui::End();
}

SGE_GUI_END
