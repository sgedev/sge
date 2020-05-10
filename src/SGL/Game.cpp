//
//
#include <SGL/Game.hpp>

SGL_BEGIN

Game::Game(void) :
	m_window(NULL),
	m_renderer(NULL)
{
}

Game::~Game(void)
{
}

bool Game::init(void)
{
	m_window = createWindow();

	m_renderer = createRenderer();
	if (m_renderer != NULL)
		m_scene.addListener(m_renderer);

	m_physics = createPhysics();
	if (m_physics != NULL)
		m_scene.addListener(m_physics);

	return true;
}

bool Game::handleEvent(const SDL_Event &event)
{
	bool ret = false;

	switch (event.type) {
	case SDL_WINDOWEVENT:
		if (m_window != NULL)
			ret = m_window->handleEvent(event.window);
		break;
	default:
		break;
	}

	return ret;
}

void Game::frame(float elapsed)
{
	m_scene.update(elapsed);
}

SGL_END
