//
//
#include <sge/game.hpp>

SGE_BEGIN

static inline Game::Task_t *GameTaskFromLua(lua_State *L)
{
	return (Game::Task_t *)lua_getextraspace(L);
}

static inline lua_State *GameTaskToLua(Game::Task_t *T)
{
	return (lua_State *)CX_PMOVB(T, LUA_EXTRASPACE);
}

static inline void GameAttach(Game *G, lua_State *L)
{
	GameTaskFromLua(L)->data = G;
}

static inline Game *GameFromTask(Game::Task_t *T)
{
	return (Game *)(T->data);
}

static inline Game *GameFromLua(lua_State *L)
{
	return GameFromTask(GameTaskFromLua(L));
}

Game::Game(void)
	: m_L(NULL)
	, m_current_trap(TRAP_INVALID)
{
}

Game::~Game(void)
{
}

bool Game::init(const char *root)
{
	SGE_ASSERT(m_L == NULL);
	
	return true;

	m_running = true;

	std::promise<bool> init_result;
	m_thread = std::thread(&Game::tmain, this, &init_result);
	if (!m_thread.joinable())
		return false;

	auto init_done = init_result.get_future();
	init_done.wait();

	if (!init_done.get())
		return false;

	m_state = STATE_LOADING;

	return true;
}

void Game::shutdown(void)
{
	return;

	if (!m_running)
		return;

	SGE_ASSERT(m_L != NULL);

	uv_async_send(&m_quit_async);

	if (m_thread.joinable())
		m_thread.join();
}

bool Game::handleEvent(const Event *evt)
{
	return false;
}

void Game::update(float elapsed)
{
	m_scene.update(elapsed);

	if (m_current_trap == TRAP_INVALID)
		return;

	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_current_trap == TRAP_INVALID)
		return;

	m_current_trap_result = trapBE(m_current_trap_L, m_current_trap);
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

int Game::trapFE(lua_State *L, TrapType tt)
{
	SGE_ASSERT(m_current_trap == TRAP_INVALID);
	SGE_ASSERT(m_current_trap_L == NULL);
	SGE_ASSERT(L != NULL);
	SGE_ASSERT(tt != TRAP_INVALID);

	m_current_trap = tt;
	m_current_trap_L = L;
	m_current_trap_result = TRAP_RESULT_NOT_SET;

	m_cond.wait(m_mutex);

	SGE_ASSERT(m_current_trap_result != TRAP_RESULT_NOT_SET);

	switch (m_current_trap_result) {
	case TRAP_RESULT_NOT_SUPPORT:
		luaL_error(L, "trap %d not support.", tt); // FIXME: how about the mutex if longjmp?
		break;
	}

	return m_current_trap_result;
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
	if (L == GameFromLua(L)->m_L)
		luaL_error(L, "Not in a thread");

	lua_pushthread(L);

	return 1;
}

void Game::sleepDone(uv_timer_t *timer)
{
	Task_t *T = (Task_t *)(timer->data);
	SGE_ASSERT(T != NULL);

	Game *G = GameFromTask(T);
	SGE_ASSERT(V != NULL);

	CXDeleteListNode(&G->m_task_list_sleep, &T->node);
	CXAddListTail(&G->m_task_list, &T->node);
}

int Game::trapSleep(lua_State *L)
{
	int ms = (int)luaL_checkinteger(L, 1);
	if (ms < 0)
		luaL_error(L, "bad arg");

	if (ms > 0) {
		Task_t *T = GameTaskFromLua(L);
		SGE_ASSERT(T != NULL);
		Game *G = GameFromTask(T);
		SGE_ASSERT(V != NULL);
		uv_timer_start(&T->sleep_timer, &Game::sleepDone, ms, 0);
		CXAddListTail(&G->m_task_list_sleep, &T->node);
	}

	return lua_yield(L, 0);
}

int Game::trapFpsFE(lua_State *L)
{
	Game *G = GameFromLua(L);
	SGE_ASSERT(G != NULL);

	return G->trapFE(L, TRAP_FPS);
}

int Game::trapEditorIsEnabledFE(lua_State *L)
{
	Game *G = GameFromLua(L);
	SGE_ASSERT(G != NULL);

	return G->trapFE(L, TRAP_EDITOR_IS_ENABLED);
}

void Game::quitAsync(uv_async_t *p)
{
	Game *G = (Game *)(p->data);
	G->m_running = false;
	uv_stop(&G->m_loop);
}

void Game::initLuaTraps(void)
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

bool Game::initMainTask(void)
{
	return false;
}

void Game::schedule(void)
{

}

void Game::gmain(std::promise<bool> *init_result)
{
	SGE_ASSERT(init_result != NULL);

	uv_loop_init(&m_loop);
	uv_async_init(&m_loop, &m_quit_async, &Game::quitAsync);

	initLuaTraps();

	if (!initMainTask()) {
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
	auto gp = (Game *)lua_touserdata(L, 1);
	auto init_result = (std::promise<bool> *)lua_touserdata(L, 2);

	SGE_ASSERT(init_result != NULL);

	gp->gmain(init_result);

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

	GameAttach(this, m_L);

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

SGE_END

extern "C" void SGEGameInitLua(lua_State *L)
{
}

extern "C" void SGEGameShutdownLua(lua_State *L)
{
}

extern "C" void SGEGameAddLuaTask(lua_State *L, lua_State *L1)
{
}

extern "C" void SGEGameRemoveLuaTask(lua_State *L, lua_State *L1)
{
}

extern "C" void SGEGameResumeLuaTask(lua_State *L, int n)
{
}

extern "C" void SGEGameYieldLuaTask(lua_State *L, int n)
{
}
