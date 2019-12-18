//
//
#include <QFile>
#include <QFileInfo>
#include <QMutexLocker>

#include <sge/game.hpp>

// TODO: luaL_error() will break the C++ class destructor!

SGE_BEGIN

Game::Game(void)
{
}

Game::~Game(void)
{
}

void Game::setName(const QString &name)
{
	if (m_name != name) {
		m_name = name;
		nameChanged(name);
	}
}

void Game::update(float elapsed)
{
	poll();
}

void Game::draw(Renderer::View *view)
{
	Q_ASSERT(view != Q_NULLPTR);
}

bool Game::initMainTask(lua_State *L)
{
	QString path(m_dir + "/main.lua");

	int ret = luaL_loadfile(L, path.toStdString().c_str());
	if (ret)
		return false;

	return true;
}

int Game::trapInfo(lua_State *L)
{
	return 0;
}

int Game::trapWarning(lua_State *L)
{
	return 0;
}

int Game::trapError(lua_State *L)
{
	return 0;
}

int Game::trapName(lua_State *L)
{
	lua_pushstring(L, name().toStdString().c_str());
	return 1;
}

int Game::trapSetName(lua_State *L)
{
	setName(lua_tostring(L, 1));
}

int Game::trapFps(lua_State *L)
{
	lua_pushinteger(L, 60);
	return 1;
}

int Game::trapMaxFps(lua_State *L)
{
	lua_pushinteger(L, 60);
	return 1;
}

int Game::trapSetMaxFps(lua_State *L)
{
	return 0;
}

SGE_END
