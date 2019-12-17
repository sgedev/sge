//
//
#include <QFile>
#include <QFileInfo>

#include <sge/game.hpp>

// TODO: luaL_error() will break the C++ class destructor!

SGE_BEGIN

Game::Game(void)
{
}

Game::~Game(void)
{
}

void Game::update(float elapsed)
{
}

void Game::draw(Renderer::View *view)
{
	Q_ASSERT(view != Q_NULLPTR);
}

void Game::luaInitHook(lua_State *L)
{
}

void Game::luaShutdownHook(lua_State *L)
{

}

void Game::luaAddTaskHook(lua_State *L, lua_State *L1)
{
	Game *game = gameFromLua(L);
	Context *ctx = contextFromLua(L1);

	Task *task = new Task; // TODO use a task pool?
	if (task == Q_NULLPTR)
		luaL_error(L, "Failed to create SGE::Task.");

	task->L = L1;

	connect(&task->sleepTimer, &QTimer::timeout, [=]() { game->onTaskReady(task); });

	ctx->game = (void *)game;
	ctx->task = (void *)task;

	cxResetListNode(&ctx->node);
	cxAddListTail(&game->m_readyTaskList, &ctx->node);
}

void Game::luaRemoveTaskHook(lua_State *L, lua_State *L1)
{
	Game *game = gameFromLua(L);
	Context *ctx = contextFromLua(L1);

	Task *task = taskFromContext(ctx);
	delete task;

	ctx->task = Q_NULLPTR;
	ctx->game = Q_NULLPTR;

	cxUnlinkListNode(&ctx->node);
}

void Game::luaResumeTaskHook(lua_State *L, int n)
{
}

void Game::luaYieldTaskHook(lua_State *L, int n)
{
	Context *ctx = contextFromLua(L);
	Game *game = gameFromContext(ctx);

	cxUnlinkListNode(&ctx->node);
	cxAddListTail(&game->m_sleepTaskList, &ctx->node);
}

void Game::run(void)
{
	QFileInfo fi(m_dir);
	if (!fi.isDir()) {
		trapLog(LogError, "Invalid game dir '" + m_dir + "'.\n");
		return;
	}

	lua_State *L = luaL_newstate();
	if (L == Q_NULLPTR) {
		trapLog(LogError, "Failed to initializing lua.\n");
		return;
	}

	Context *ctx = contextFromLua(L);
	cxResetListNode(&ctx->node);
	ctx->game = (void *)this;
	ctx->task = Q_NULLPTR;

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

	game->main(L);

	return 0;
}

void Game::main(lua_State *L)
{
	initExports(L);

	if (!initMainTask(L)) {
		trapLog(LogError, "Failed to init main task...\n");
		return;
	}

	trapLog(LogInfo, "Game is running...\n");

	m_running = true;

	while (m_running) {
		sched(L);
		exec();
	}

	trapLog(LogInfo, "Game exited.\n");
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

bool Game::initMainTask(lua_State *L)
{
	QString path(m_dir + "/main.lua");

	lua_State *mainTask = lua_newthread(L);
	if (mainTask == NULL) {
		trapLog(LogError, "Faile to create game main task.\n");
		return false;
	}

	int ret = luaL_loadfile(mainTask, path.toStdString().c_str());
	if (ret) {
		trapLog(LogError, "Failed to load game main task from '" + path + "'.\n");
		return false;
	}

	m_mainTask = taskFromLua(mainTask);

	return true;
}

void Game::sched(lua_State *L)
{
	CXListNode *node;
	Context *ctx;
	Task *task;

	for (;;) {
		node = cxDeleteListHead(&m_readyTaskList);
		if (node == cxGetListKnot(&m_readyTaskList))
			break;
		ctx = CX_MEMBEROF(node, Context, node);
		task = taskFromContext(ctx);
		int ret = lua_resume(task->L, L, 0);
		if (ret != LUA_YIELD && ret != LUA_OK)
			luaL_error(L, "LUA_ERROR: %s\n", luaL_checkstring(L, -1));
	}
}

int Game::luaVersion(lua_State *L)
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

int Game::luaInfo(lua_State *L)
{
	gameFromLua(L)->trapLog(LogInfo, luaL_checkstring(L, 1));
	return 0;
}

int Game::luaWarning(lua_State *L)
{
	gameFromLua(L)->trapLog(LogWarning, luaL_checkstring(L, 1));
	return 0;
}

int Game::luaError(lua_State *L)
{
	gameFromLua(L)->trapLog(LogError, luaL_checkstring(L, 1));
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
	Context *ctx= contextFromLua(task->L);
	Game *game = gameFromContext(ctx);

	cxUnlinkListNode(&ctx->node);
	cxAddListTail(&game->m_readyTaskList, &ctx->node);

	QThread::exit(0);
}

SGE_END

extern "C" void SGEGameLuaInitHook(lua_State *L)
{
	SGE::Game::luaInitHook(L);
}

extern "C" void SGEGameLuaShutdownHook(lua_State *L)
{
	SGE::Game::luaShutdownHook(L);
}

extern "C" void SGEGameLuaAddTaskHook(lua_State *L, lua_State *L1)
{
	SGE::Game::luaAddTaskHook(L, L1);
}

extern "C" void SGEGameLuaRemoveTaskHook(lua_State *L, lua_State *L1)
{
	SGE::Game::luaRemoveTaskHook(L, L1);
}

extern "C" void SGEGameLuaResumeTaskHook(lua_State *L, int n)
{
	SGE::Game::luaResumeTaskHook(L, n);
}

extern "C" void SGEGameLuaYieldTaskHook(lua_State *L, int n)
{
	SGE::Game::luaYieldTaskHook(L, n);
}
