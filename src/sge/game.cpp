//
//
#include <QFile>
#include <QFileInfo>
#include <QMutexLocker>

#include <sge/game.hpp>

#define SGE_SCENE_TYPE "sge.scene"
#define SGE_LEVEL_TYPE "sge.level"

SGE_BEGIN

Game::Game(void)
	: m_running(false)
	, m_trapType(TrapNull)
	, m_trapArgs(Q_NULLPTR)
	, m_trapResult(0)
{
}

Game::~Game(void)
{
}

bool Game::setPath(const QString &path)
{
	if (isRunning())
		return false;

	QFileInfo fi(path);
	if (!fi.isDir())
		return false;

	fi.setFile(path + "/" + "main.lua");
	if (!fi.isFile())
		return false;

	m_path = path;

	return true;
}

void Game::update(float elapsed)
{
	// TODO

	if (!isRunning())
		return;

	if (m_trapType == TrapNull)
		return;

	QMutexLocker locker(&m_mutex);

	Q_ASSERT(m_trapType != TrapNull);
	Q_ASSERT(m_trapArgs != Q_NULLPTR);

	switch (m_trapType) {
	case TrapInfo:
		m_trapResult = trapInfo(m_trapArgs);
		break;
	case TrapWarning:
		m_trapResult = trapWarning(m_trapArgs);
		break;
	case TrapError:
		m_trapResult = trapError(m_trapArgs);
		break;
	case TrapName:
		m_trapResult = trapName(m_trapArgs);
		break;
	case TrapSetName:
		m_trapResult = trapSetName(m_trapArgs);
		break;
	case TrapCreateScene:
		m_trapResult = trapCreateScene(m_trapArgs);
		break;
	default:
		break;
	}

	m_trapType = TrapNull;
	m_trapArgs = Q_NULLPTR;

	m_cond.wakeAll();
}

void Game::draw(Renderer::View *view)
{
	Q_ASSERT(view != Q_NULLPTR);
}

void Game::initLua(lua_State *L)
{
}

void Game::shutdownLua(lua_State *L)
{
}

void Game::addLuaTask(lua_State *L, lua_State *L1)
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

	cx_list_node_reset(&ctx->node);
	cx_list_add_tail(&game->m_readyTaskList, &ctx->node);
}

void Game::removeLuaTask(lua_State *L, lua_State *L1)
{
	Game *game = gameFromLua(L);
	Context *ctx = contextFromLua(L1);

	Task *task = taskFromContext(ctx);
	delete task;

	ctx->game = Q_NULLPTR;
	ctx->task = Q_NULLPTR;

	cx_list_node_unlink(&ctx->node);
}

void Game::resumeLuaTask(lua_State *L, int n)
{
}

void Game::yieldLuaTask(lua_State *L, int n)
{
	Context *ctx = contextFromLua(L);
	Game *game = gameFromContext(ctx);

	cx_list_node_unlink(&ctx->node);
	cx_list_add_tail(&game->m_sleepTaskList, &ctx->node);
}

void Game::run(void)
{
	QMutexLocker locker(&m_mutex);

	lua_State *L = luaL_newstate();
	if (L == Q_NULLPTR)
		return;

	Context *ctx = contextFromLua(L);
	cx_list_node_reset(&ctx->node);
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

	lua_State *mainTask = lua_newthread(L);
	if (mainTask == NULL)
		return;

	if (!initMainTask(mainTask))
		return;

	m_running = true;

	while (m_running) {
		scheduleTasks(L);
		exec();
	}
}

void Game::initExports(lua_State *L)
{
	luaL_newmetatable(L, SGE_SCENE_TYPE);
	luaL_newmetatable(L, SGE_LEVEL_TYPE);

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

	lua_pushcfunction(L, &Game::luaName);
	lua_setfield(L, -2, "name");

	lua_pushcfunction(L, &Game::luaSetName);
	lua_setfield(L, -2, "setName");

	lua_pushcfunction(L, &Game::luaCreateScene);
	lua_setfiled(L, -2, "createScene");

	lua_setglobal(L, "sge");
}

bool Game::initMainTask(lua_State *L)
{
	return true;
}

void Game::scheduleTasks(lua_State *L)
{
	cx_list_node_t *node;
	Context *ctx;
	Task *task;

	for (;;) {
		node = cx_list_del_head(&m_readyTaskList);
		if (node == cx_list_knot(&m_readyTaskList))
			break;
		ctx = CX_MEMBEROF(node, Context, node);
		task = taskFromContext(ctx);
		int ret = lua_resume(task->L, L, 0);
		if (ret != LUA_YIELD && ret != LUA_OK)
			luaL_error(L, "LUA_ERROR: %s\n", luaL_checkstring(L, -1));
	}
}

int Game::doTrap(lua_State *L, TrapType trap)
{
	Q_ASSERT(L != Q_NULLPTR);
	Q_ASSERT(trap != TrapNull);

	Game *game = gameFromLua(L);
	Q_ASSERT(QThread::currentThread() == static_cast<QThread *>(game));

	Q_ASSERT(game->m_trapType == TrapNull);
	Q_ASSERT(game->m_trapArgs == Q_NULLPTR);

	game->m_trapType = trap;
	game->m_trapArgs = L;

	while (game->m_trapType != TrapNull)
		game->m_cond.wait(&game->m_mutex);

	return game->m_trapResult;
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
	luaL_checkstring(L, 1);

	return doTrap(L, TrapInfo);
}

int Game::trapInfo(lua_State *L)
{
	return 0;
}

int Game::luaWarning(lua_State *L)
{
	luaL_checkstring(L, 1);

	return doTrap(L, TrapWarning);
}

int Game::trapWarning(lua_State *L)
{
	return 0;
}

int Game::luaError(lua_State *L)
{
	luaL_checkstring(L, 1);

	return doTrap(L, TrapError);
}

int Game::trapError(lua_State *L)
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

int Game::luaName(lua_State *L)
{
	return doTrap(L, TrapName);
}

int Game::trapName(lua_State *L)
{
	return 0;
}

int Game::luaSetName(lua_State *L)
{
	luaL_checkstring(L, 1);

	return doTrap(L, TrapSetName);
}

int Game::trapSetName(lua_State *L)
{
	return 0;
}

int Game::luaCreateScene(lua_State *L)
{
	luaL_checkstring(L, 1);

	return doTrap(L, TrapCreateScene);
}

int Game::trapCreateScene(lua_State *L)
{
	const char *name = lua_tostring(L, 1);

	Scene *p = (Scene *)lua_newuserdata(L, sizeof(Scene));
	if (p == Q_NULLPTR)
		return makeTrapError(L, "Out of memory.");

	luaL_getmetatable(L, SGE_SCENE_TYPE) ;
	lua_setmetatable(L, -2);

	new(scene) Scene();

	ScenePtr scene(p);
	m_scenes << scene;

	return 1;
}

void Game::onTaskReady(Task *task)
{
	Context *ctx= contextFromLua(task->L);
	Game *game = gameFromContext(ctx);

	cx_list_node_unlink(&ctx->node);
	cx_list_add_tail(&game->m_readyTaskList, &ctx->node);

	QThread::exit(0);
}

SGE_END

extern "C" void SGEGameInitLua(lua_State *L)
{
	SGE::Game::initLua(L);
}

extern "C" void SGEGameShutdownLua(lua_State *L)
{
	SGE::Game::shutdownLua(L);
}

extern "C" void SGEGameAddLuaTask(lua_State *L, lua_State *L1)
{
	SGE::Game::addLuaTask(L, L1);
}

extern "C" void SGEGameRemoveLuaTask(lua_State *L, lua_State *L1)
{
	SGE::Game::removeLuaTask(L, L1);
}

extern "C" void SGEGameResumeLuaTask(lua_State *L, int n)
{
	SGE::Game::resumeLuaTask(L, n);
}

extern "C" void SGEGameYieldLuaTask(lua_State *L, int n)
{
	SGE::Game::yieldLuaTask(L, n);
}

