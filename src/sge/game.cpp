//
//
#include <QFile>
#include <QFileInfo>

#include <sge/game.hpp>

// TODO: luaL_error() will break the C++ class destructor!

SGE_BEGIN

template <typename FN>
int Game::luaVersion(lua_State *L)
{
	QString errorMsg;
	int rc = gameFromLua(L)->luaVersionTrap(L, errorMsg);
	if (rc < 0)
		luaL_error(L, errorMsg.toStdString().c_str());
}

Game::Game(void)
{
}

Game::~Game(void)
{
}

bool Game::setDir(const QString &dirname)
{
	QFileInfo fi(dirname);
	if (!fi.isDir())
		return false;

	m_dir = dirname;

	return true;
}

void Game::update(float elapsed)
{
}

void Game::draw(Renderer::View *view)
{
	Q_ASSERT(view != Q_NULLPTR);
}

void Game::luaInit(lua_State *L)
{
}

void Game::luaShutdown(lua_State *L)
{

}

void Game::luaAddTask(lua_State *L, lua_State *L1)
{
	Game *game = gameFromLua(L);
	Info *info = infoFromLua(L1);

	Task *task = new Task; // TODO use a task pool?
	if (task == Q_NULLPTR)
		luaL_error(L, "Failed to create SGE::Task.");

	task->L = L1;

	connect(&task->sleepTimer, &QTimer::timeout, [&game, &task](void) { game->onTaskReady(task); });

	info->game = (void *)game;
	info->task = (void *)task;

	cxResetListNode(&info->node);
	cxAddListTail(&game->m_readyTaskList, &info->node);
}

void Game::luaRemoveTask(lua_State *L, lua_State *L1)
{
	Game *game = gameFromLua(L);
	Info *info = infoFromLua(L1);

	info->task = Q_NULLPTR;
	info->game = Q_NULLPTR;

	Task *task = taskFromInfo(info);
	delete task;

	cxUnlinkListNode(&info->node);
}

void Game::luaResumeTask(lua_State *L, int n)
{
}

void Game::luaYieldTask(lua_State *L, int n)
{
	Info *info = infoFromLua(L);
	Game *game = gameFromInfo(info);

	cxUnlinkListNode(&info->node);
	cxAddListTail(&game->m_sleepTaskList, &info->node);
}

void Game::run(void)
{
	QFileInfo fi(m_dir);
	if (!fi.isDir()) {
		//trapFatal("Invalid game dir '" + m_dir + "'.\n");
		return;
	}

	lua_State *L = luaL_newstate();
	if (L == Q_NULLPTR) {
		//trapFatal("Failed to initializing lua.\n");
		return;
	}

	Info *info = infoFromLua(L);
	cxResetListNode(&info->node);
	info->game = (void *)this;
	info->task = Q_NULLPTR;

	lua_pushlightuserdata(L, this);
	lua_pushcfunction(L, &Game::luaMain);

	int rc = lua_pcall(L, 1, 0, 0);

	lua_close(L);
}

int Game::luaMain(lua_State *L)
{
	luaL_openlibs(L);

	Game *game = (Game *)lua_touserdata(L, 1);
	Q_ASSERT(game != Q_NULLPTR);

	return game->gameMain(L);
}

int Game::gameMain(lua_State *L)
{
	initExports(L);
	initMainTask(L);

	trapInfo("Game is running...\n");

	m_running = true;

	while (m_running) {
		sched(L);
		exec();
	}

	trapInfo("Game exited.\n");

	return 1;
}

void Game::initExports(lua_State *L)
{
	lua_newtable(L);

	lua_pushcfunction(L, &Game::luaVersion);
	lua_setfield(L, -2, "version");

	lua_pushcfunction(L, &Game::luaInfo);
	lua_setfield(L, -2, "info");

	lua_pushcfunction(L, &Game::luaWarning);
	lua_setfield(L, -2, "warning");

	lua_pushcfunction(L, &Game::luaError);
	lua_setfield(L, -2, "error");

	lua_pushcfunction(L, &Game::luaTask);
	lua_setfield(L, -2, "task");

	lua_pushcfunction(L, &Game::luaSleep);
	lua_setfield(L, -2, "sleep");

	lua_pushcfunction(L, &Game::luaFps);
	lua_setfield(L, -2, "fps");

	lua_pushcfunction(L, &Game::luaMaxFps);
	lua_setfield(L, -2, "fpsMax");

	lua_pushcfunction(L, &Game::luaSetMaxFps);
	lua_setfield(L, -2, "setFpsMax");

	lua_pushcfunction(L, &Game::luaName);
	lua_setfield(L, -2, "name");

	lua_pushcfunction(L, &Game::luaSetName);
	lua_setfield(L, -2, "setName");

	lua_setglobal(L, "sge");
}

void Game::initMainTask(lua_State *L)
{
}

void Game::sched(lua_State *L)
{
	CXListNode *node;
	Info *info;
	Task *task;

	for (;;) {
		node = cxDeleteListHead(&m_readyTaskList);
		if (node == cxGetListKnot(&m_readyTaskList))
			break;
		info = CX_MEMBEROF(node, Info, node);
		task = taskFromInfo(info);
		int ret = lua_resume(task->L, L, 0);
		if (ret != LUA_YIELD && ret != LUA_OK)
			luaL_error(L, "LUA_ERROR: %s\n", luaL_checkstring(L, -1));
	}
}

int Game::luaVersionTrap(lua_State *L, QString &errorMsg)
{
	lua_newtable(L);

	lua_pushinteger(L, SGE_VERSION_MAJOR);
	lua_rawseti(L, -1, 0);

	lua_pushinteger(L, SGE_VERSION_MINOR);
	lua_rawseti(L, -1, 1);

	lua_pushinteger(L, SGE_VERSION_PATCH);
	lua_rawseti(L, -1, 2);

	return 1;
}

int Game::luaVersion(lua_State *L)
{
	QString errorMsg;
	int rc = gameFromLua(L)->luaVersionTrap(L, errorMsg);
	if (rc < 0)
		luaL_error(L, errorMsg.toStdString().c_str());
}

int Game::luaInfo(lua_State *L)
{
	return 0;
}

int Game::luaWarning(lua_State *L)
{
	return 0;
}

int Game::luaError(lua_State *L)
{
	return 0;
}

int Game::luaTask(lua_State *L)
{
	luaL_argcheck(L, lua_isfunction(L, 1), 1, "not a function");

	lua_State *L1 = lua_newthread(L);
	if (L1 != NULL) {
		lua_pushvalue(L, 1);
		lua_xmove(L, L1, 1);
	} else
		lua_pushnil(L);

	return 1;
}

int Game::luaCurrent(lua_State *L)
{
	lua_pushthread(L);
	return 1;
}

int Game::luaSleep(lua_State *L)
{
	Task *task = taskFromLua(L);
	
	task->sleepTimer.start(luaL_checkinteger(L, 1));

	return lua_yield(L, 0);
}

int Game::luaFps(lua_State *L)
{
	lua_pushinteger(L, gameFromLua(L)->trapFps());
	return 1;
}

int Game::luaMaxFps(lua_State *L)
{
	lua_pushinteger(L, gameFromLua(L)->trapMaxFps());
	return 1;
}

int Game::luaSetMaxFps(lua_State *L)
{
	int fps = luaL_checkinteger(L, 1);
	int ret = gameFromLua(L)->trapSetMaxFps(fps);

	lua_pushinteger(L, ret);
	return 1;
}

int Game::luaName(lua_State *L)
{
	Game *game = gameFromLua(L);
	lua_pushstring(L, game->m_name.toStdString().c_str());
	return 1;
}

int Game::luaSetName(lua_State *L)
{
	gameFromLua(L)->m_name = luaL_checkstring(L, 1);
	return 0;
}

void Game::onTaskReady(Task *task)
{
	Info *info = infoFromLua(task->L);
	Game *game = gameFromInfo(info);

	cxUnlinkListNode(&info->node);
	cxAddListTail(&game->m_readyTaskList, &info->node);

	exit(0);
}

SGE_END

extern "C" void SGEGameLuaInit(lua_State *L)
{
	SGE::Game::luaInit(L);
}

extern "C" void SGEGameLuaShutdown(lua_State *L)
{
	SGE::Game::luaShutdown(L);
}

extern "C" void SGEGameLuaAddTask(lua_State *L, lua_State *L1)
{
	SGE::Game::luaAddTask(L, L1);
}

extern "C" void SGEGameLuaRemoveTask(lua_State *L, lua_State *L1)
{
	SGE::Game::luaRemoveTask(L, L1);
}

extern "C" void SGEGameLuaResumeTask(lua_State *L, int n)
{
	SGE::Game::luaResumeTask(L, n);
}

extern "C" void SGEGameLuaYieldTask(lua_State *L, int n)
{
	SGE::Game::luaYieldTask(L, n);
}
