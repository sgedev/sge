//
//
#include <sge/game.hpp>

SGE_BEGIN

static inline void game_attach_lua(game *G, lua_State *L)
{
	*((game **)lua_getextraspace(L)) = G;
}

static inline game *game_from_lua(lua_State *L)
{
	return *((game **)lua_getextraspace(L));
}

game::game(void)
	: m_L(NULL)
	, m_state(STATE_IDLE)
{
}

game::~game(void)
{
	if (m_L != NULL)
		shutdown();
}

bool game::init(traps *p)
{
	SGE_ASSERT(m_L == NULL);
	SGE_ASSERT(p != NULL);

	if (!m_scene.init())
		return false;

	m_traps = p;

	std::promise<bool> init_result;
	m_thread = std::thread(&game::tmain, this, &init_result);
	if (!m_thread.joinable()) {
		m_scene.shutdown();
		return false;
	}

	auto init_done = init_result.get_future();
	init_done.wait();

	if (!init_done.get()) {
		m_scene.shutdown();
		return false;
	}

	m_state = STATE_LOADING;

	return true;
}

void game::shutdown(void)
{
	SGE_ASSERT(m_L != NULL);

	uv_async_send(&m_quit_async);

	if (m_thread.joinable()) {
		SGE_LOGD("waiting for game thread exited...\n");
		m_thread.join();
	}

	m_scene.shutdown();
}

void game::handle_event(const SDL_Event &event)
{
	m_input.handle_event(event);
}

void game::update(float elapsed)
{
	m_scene.update(elapsed);
}

void game::draw(view &v)
{
	m_scene.draw(v);
}

void game::quit_async(uv_async_t *p)
{
	game *_this = (game *)(p->data);
	_this->m_running = false;
	uv_stop(p->loop);
}

void game::gmain(std::promise<bool> *init_result)
{
	SGE_ASSERT(init_result != NULL);

	uv_loop_init(&m_loop);
	uv_async_init(&m_loop, &m_quit_async, &game::quit_async);
	m_quit_async.data = this;
	m_running = true;

	init_traps();

	m_input.init();

	init_result->set_value(true);

	while (m_running) {
		uv_run(&m_loop, UV_RUN_DEFAULT);
		schedule();
	}

	m_input.shutdown();

	uv_loop_close(&m_loop);
}

int game::pmain(lua_State *L)
{
	auto _this = (game *)lua_touserdata(L, 1);
	auto init_result = (std::promise<bool> *)lua_touserdata(L, 2);

	SGE_ASSERT(_this != NULL);
	SGE_ASSERT(init_result != NULL);

	_this->gmain(init_result);
}

void game::tmain(std::promise<bool> *init_result)
{
	int ret;

	SGE_ASSERT(m_L == NULL);

	m_L = luaL_newstate();
	if (m_L == NULL) {
		init_result->set_value(false);
		return;
	}

	game_attach_lua(this, m_L);

	lua_pushcfunction(m_L, &game::pmain);
	lua_pushlightuserdata(m_L, this);
	lua_pushlightuserdata(m_L, init_result);

	ret = lua_pcall(m_L, 2, 1, 0);
	// TODO

	ret = lua_toboolean(m_L, -1);
	// TODO

	lua_close(m_L);
	m_L = NULL;
}

void game::init_traps(void)
{
	lua_newtable(m_L);

	lua_setglobal(m_L, "sge");
}

void game::schedule(void)
{
}

SGE_END

extern "C" void sge_game_lua_open(lua_State *L)
{
}

extern "C" void sge_game_lua_close(lua_State *L)
{
}

extern "C" void sge_game_lua_thread(lua_State *L, lua_State *L1)
{
	
}

extern "C" void sge_game_lua_free(lua_State *L, lua_State *L1)
{
}

extern "C" void sge_game_lua_resume(lua_State *L, int n)
{
}

extern "C" void sge_game_lua_yield(lua_State *L, int n)
{
}

