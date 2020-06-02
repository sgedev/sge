//
//
#include <SGE/Game.hpp>

SGE_BEGIN

Game::Game(QObject *parent):
	QAbstractItemModel(parent)
{
}

Game::~Game(void)
{
}

bool Game::init(const QString &baseDir)
{
	if (!m_resourceManager.init(baseDir))
		return false;

	return true;
}

void Game::update(float elapsed)
{
}

int	Game::columnCount(const QModelIndex & parent) const
{
	return 2;
}

QVariant Game::data(const QModelIndex & index, int role) const
{
	return QVariant();
}

QModelIndex Game::index(int row, int column, const QModelIndex & parent) const
{
	return QModelIndex();
}

QModelIndex	Game::parent(const QModelIndex & index) const
{
	return QModelIndex();
}

int	Game::rowCount(const QModelIndex & parent) const
{
	return 0;
}

Game::ObjectID Game::objectNew(const QString &name, ObjectID parent)
{
	return 0;
}

void Game::objectDestroy(ObjectID obj)
{
}

SGE_END
