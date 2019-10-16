//
//
#include <sge/game.hpp>

SGE_BEGIN

static inline Game::Task_t *luaToGameTask(lua_State *L)
{
	return (Game::Task_t *)lua_getextraspace(L);
}

static inline lua_State *luaFromGameTask(Game::Task_t *T)
{
	return (lua_State *)CX_PMOVB(T, LUA_EXTRASPACE);
}

static inline void luaAttachGame(lua_State *L, Game *G)
{
	luaToGameTask(L)->data = G;
}

static inline Game *gameFromTask(Game::Task_t *T)
{
	return (Game *)(T->data);
}

static inline Game *gameFromLua(lua_State *L)
{
	return gameFromTask(luaToGameTask(L));
}

Game::Game(void)
	: m_root(NULL)
	, m_L(NULL)
	, m_taskCurrent(NULL)
	, m_currentTrap(TRAP_INVALID)
{
}

Game::~Game(void)
{
	if (m_L != NULL)
		shutdown();
}

bool Game::init(ttvfs::Root *root)
{
	SGE_ASSERT(root != NULL);
	SGE_ASSERT(m_L == NULL);
	SGE_ASSERT(m_state == STATE_IDLE);

	m_root = root;
	m_running = true;
	m_state = STATE_INITIALIZING;

	std::promise<bool> init_result;

	m_luaThread = std::thread(&Game::tmain, this, &init_result);
	if (!m_luaThread.joinable())
		return false;

	auto init_done = init_result.get_future();
	init_done.wait();

	if (!init_done.get())
		return false;

	return true;
}

void Game::shutdown(void)
{
	SGE_ASSERT(m_running);
	SGE_ASSERT(m_L != NULL);

	uv_async_send(&m_quitAsync);

	if (m_luaThread.joinable())
		m_luaThread.join();
}

bool Game::handleEvent(const Event *evt)
{
	return false;
}

void Game::update(float elapsed)
{
	m_scene.update(elapsed);

	if (m_currentTrap == TRAP_INVALID)
		return;

	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_currentTrap == TRAP_INVALID)
		return;

	m_currentTrapResult = trapBE(m_currentTrapLua, m_currentTrap);
	m_cond.notify_all();
}

void Game::draw(View *v)
{
	switch (m_state) {
	case STATE_IDLE:
		break;
	case STATE_LOADING:
		break;
	case STATE_READY:
		break;
	case STATE_PLAYING:
		break;
	}

	m_scene.draw(v);
}

void Game::addLuaTask(lua_State *L)
{
	Task_t *T = luaToGameTask(L);
	cxResetListNode(&T->node);
	cxAddListTail(&m_taskList, &T->node);	
	luaAttachGame(L, this);
}

void Game::removeLuaTask(lua_State *L)
{
	Task_t *T = luaToGameTask(L);
	cxUnlinkListNode(&T->node);	
}

void Game::resumeLuaTask(lua_State *L, int n)
{
	Task_t *T = luaToGameTask(L);
	m_taskCurrent = T;
}

void Game::yieldLuaTask(lua_State *L, int n)
{
	m_taskCurrent = NULL;
}

void Game::quitAsync(uv_async_t *p)
{
	Game *G = (Game *)(p->data);
	G->m_running = false;
	uv_stop(&G->m_loop);
}

void Game::initTraps(void)
{
	lua_newtable(m_L);

	lua_pushcfunction(m_L, &Game::trapVersion);
	lua_setfield(m_L, -2, "version");

	lua_pushcfunction(m_L, &Game::trapTask);
	lua_setfield(m_L, -2, "task");

	lua_pushcfunction(m_L, &Game::trapCurrent);
	lua_setfield(m_L, -2, "current");

	lua_pushcfunction(m_L, &Game::trapSleep);
	lua_setfield(m_L, -2, "sleep");

	lua_pushcfunction(m_L, &Game::trapFpsFE);
	lua_setfield(m_L, -2, "fps");

	lua_newtable(m_L);

	lua_pushcfunction(m_L, &Game::trapEditorIsEnabledFE);
	lua_setfield(m_L, -2, "enabled");

	lua_setfield(m_L, -2, "editor");

	lua_setglobal(m_L, "sge");
}

bool Game::loadMainTask(void)
{
	return true;
}

void Game::schedule(void)
{

}

void Game::gmain(std::promise<bool> *init_result)
{
	SGE_ASSERT(init_result != NULL);

	uv_loop_init(&m_loop);

	uv_async_init(&m_loop, &m_quitAsync, &Game::quitAsync);
	m_quitAsync.data = this;

	initTraps();

	if (!loadMainTask()) {
		init_result->set_value(false);
		return;
	}

	init_result->set_value(true);

	while (m_running) {
		uv_run(&m_loop, UV_RUN_DEFAULT);
		schedule();
	}

	uv_loop_close(&m_loop);
}

int Game::pmain(lua_State *L)
{
	auto G = (Game *)lua_touserdata(L, 1);
	auto init_result = (std::promise<bool> *)lua_touserdata(L, 2);

	SGE_ASSERT(G != NULL);
	SGE_ASSERT(init_result != NULL);

	G->gmain(init_result);

	return 0;
}

void Game::tmain(std::promise<bool> *init_result)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	SGE_ASSERT(m_L == NULL);

	m_L = luaL_newstate();
	if (m_L == NULL) {
		init_result->set_value(false);
		return;
	}

	luaAttachGame(m_L, this);

	lua_pushcfunction(m_L, &pmain);
	lua_pushlightuserdata(m_L, this);
	lua_pushlightuserdata(m_L, init_result);

	int ret;

	ret = lua_pcall(m_L, 2, 1, 0);
	// TODO

	ret = lua_toboolean(m_L, -1);
	// TODO

	lua_close(m_L);
	m_L = NULL;
}

int Game::trapFE(lua_State *L, TrapType tt)
{
	SGE_ASSERT(m_currentTrap == TRAP_INVALID);
	SGE_ASSERT(m_currentTrapLua == NULL);
	SGE_ASSERT(L != NULL);
	SGE_ASSERT(tt != TRAP_INVALID);

	m_currentTrap = tt;
	m_currentTrapLua = L;
	m_currentTrapResult = TRAP_RESULT_NOT_SET;

	m_cond.wait(m_mutex);

	SGE_ASSERT(m_currentTrapResult != TRAP_RESULT_NOT_SET);

	switch (m_currentTrapResult) {
	case TRAP_RESULT_NOT_SUPPORT:
		luaL_error(L, "trap %d not support.", tt); // FIXME: how about the mutex if longjmp?
		break;
	}

	return m_currentTrapResult;
}

int Game::trapBE(lua_State *L, TrapType tt)
{
	int ret;

	switch (tt) {
	case TRAP_FPS:
		ret = trapFps(L);
		break;
	case TRAP_EDITOR_IS_ENABLED:
		ret = trapEditorIsEnabled(L);
		break;
	default:
		ret = TRAP_RESULT_NOT_SUPPORT;
		break;
	}

	return ret;
}

int Game::trapVersion(lua_State *L)
{
	lua_newtable(L);

	lua_pushinteger(L, SGE_VERSION_MAJOR);
	lua_rawseti(L, -2, 0);

	lua_pushinteger(L, SGE_VERSION_MAJOR);
	lua_rawseti(L, -2, 1);

	lua_pushinteger(L, SGE_VERSION_MAJOR);
	lua_rawseti(L, -2, 2);

	return 1;
}

int Game::trapTask(lua_State *L)
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

int Game::trapCurrent(lua_State *L)
{
	if (L == gameFromLua(L)->m_L)
		luaL_error(L, "Not in a thread");

	lua_pushthread(L);

	return 1;
}

void Game::sleepDone(uv_timer_t *timer)
{
	Task_t *T = (Task_t *)(timer->data);
	SGE_ASSERT(T != NULL);

	Game *G = gameFromTask(T);
	SGE_ASSERT(V != NULL);

	cxDeleteListNode(&G->m_taskListSleep, &T->node);
	cxAddListTail(&G->m_taskList, &T->node);
}

int Game::trapSleep(lua_State *L)
{
	int ms = (int)luaL_checkinteger(L, 1);
	if (ms < 0)
		luaL_error(L, "bad arg");

	if (ms > 0) {
		Task_t *T = luaToGameTask(L);
		SGE_ASSERT(T != NULL);
		Game *G = gameFromTask(T);
		SGE_ASSERT(V != NULL);
		uv_timer_start(&T->sleepTimer, &Game::sleepDone, ms, 0);
		cxAddListTail(&G->m_taskListSleep, &T->node);
	}

	return lua_yield(L, 0);
}

int Game::trapFpsFE(lua_State *L)
{
	Game *G = gameFromLua(L);
	SGE_ASSERT(G != NULL);

	return G->trapFE(L, TRAP_FPS);
}

int Game::trapEditorIsEnabledFE(lua_State *L)
{
	Game *G = gameFromLua(L);
	SGE_ASSERT(G != NULL);

	return G->trapFE(L, TRAP_EDITOR_IS_ENABLED);
}

SGE_END

extern "C" void SGE_Game_InitLua(lua_State *L)
{
}

extern "C" void SGE_Game_ShutdownLua(lua_State *L)
{
}

extern "C" void SGE_Game_AddLuaTask(lua_State *L, lua_State *L1)
{
	SGE::Game *G = SGE::gameFromLua(L);
	SGE_ASSERT(G != NULL);
	G->addLuaTask(L1);
}

extern "C" void SGE_Game_RemoveLuaTask(lua_State *L, lua_State *L1)
{
	SGE::Game *G = SGE::gameFromLua(L);
	SGE_ASSERT(G != NULL);
	G->removeLuaTask(L1);
}

extern "C" void SGE_Game_ResumeLuaTask(lua_State *L, int n)
{
	SGE::Game *G = SGE::gameFromLua(L);
	SGE_ASSERT(G != NULL);
	G->resumeLuaTask(L, n);
}

extern "C" void SGE_Game_YieldLuaTask(lua_State *L, int n)
{
	SGE::Game *G = SGE::gameFromLua(L);
	SGE_ASSERT(G != NULL);
	G->yieldLuaTask(L, n);
}
