//
//
#include <SGE/Editor.hpp>

SGE_BEGIN

Editor::Editor(uv_loop_t *loop):
	Player(loop)
{
}

Editor::~Editor(void)
{
}

bool Editor::start(void)
{
	if (!Player::start())
		return false;

	SDL_SetWindowResizable(m_window, SDL_TRUE);

	return true;
}

void Editor::stop(void)
{
	Player::stop();
}

void Editor::updateGui(void)
{
	Player::updateGui();

	ImGui::ShowDemoWindow();
}

SGE_END
