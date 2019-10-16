//
//
#include "game.hpp"

Game::Game(void)
{
}

Game::~Game(void)
{
}

bool Game::init(ttvfs::Root *root)
{
	if (!SGE::Game::init(root))
		return false;

	return true;
}

void Game::shutdown(void)
{
	SGE::Game::shutdown();
}

bool Game::handleEvent(const QEvent *evt)
{
	SGE::Event event;

	// TODO conv qevent into sge::event...
	//return m_game.handleEvent(evt);

	// return SGE::Game::handleEvent(&event);

	return false;
}

void Game::update(float elapsed)
{
	SGE::Game::update(elapsed);
}

void Game::draw(SGE::View *v)
{
	SGE::Game::draw(v);
}

int Game::trapFps(lua_State *L)
{
	return TRAP_RESULT_NOT_SUPPORT;
}

int Game::trapEditorIsEnabled(lua_State *L)
{
	lua_pushboolean(L, 1);
	return 1;
}

