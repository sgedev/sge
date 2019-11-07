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

bool Game::init(SGE::FileSystem *fs)
{
	if (!Client::init(fs))
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

