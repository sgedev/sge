//
//
#include <sge/game.hpp>

SGE_BEGIN

Game::Game(void)
{
}

Game::~Game(void)
{
}

void Game::update(float elapsed)
{
	m_scene.update(elapsed);
}

SGE_END

