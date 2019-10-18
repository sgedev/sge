//
//
#include <sge/editor/game.hpp>

SGE_EDITOR_BEGIN

Game::Game(QObject *parent)
	: Client(parent)
{
}

Game::~Game(void)
{
}

bool Game::init(ttvfs::Root *root)
{
	if (!Client::init(root))
		return false;

	return true;
}

void Game::shutdown(void)
{
	Client::shutdown();
}

void Game::update(float elapsed)
{
	Client::update(elapsed);
}

void Game::draw(Renderer::View *view)
{
	Client::draw(view);
}

SGE_EDITOR_END

