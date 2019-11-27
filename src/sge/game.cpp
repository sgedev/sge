//
//
#include <sge/game.hpp>

SGE_BEGIN

Game::Game(QObject *parent)
	: QObject(parent)
{
}

Game::~Game(void)
{
}

bool Game::init(Database::FileSystem *dbfs)
{
	Q_ASSERT(dbfs != Q_NULLPTR);

	if (!m_db.load(dbfs))
		return false;

	return true;
}

void Game::shutdown(void)
{
}

void Game::update(float elapsed)
{
}

void Game::draw(Renderer::View *view)
{
}

SGE_END
