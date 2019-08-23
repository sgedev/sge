//
//
#include <sge/game.hpp>

#define SGE_GAME_MAIN_RC "/init.lua"

SGE_BEGIN

game::game(void)
	: m_L(NULL)
	, m_state(STATE_IDLE)
	, m_current_trap(TRAP_TYPE_INVALID)
	, m_current_trap_L(NULL)
	, m_main_task(NULL)
{
}

game::~game(void)
{
	if (m_L != NULL)
		shutdown();
}

bool game::init(void)
{
	SGE_ASSERT(m_L == NULL);

	sge_list_reset(&m_task_list);
	sge_list_reset(&m_task_list_sleep);

	m_main_task = NULL;

	if (!m_scene.init())
		return false;

	std::lock_guard<std::mutex> lock(m_mutex);

	m_thread = std::thread(&game::tmain, this);
	if (!m_thread.joinable()) {
		m_scene.shutdown();
		return false;
	}

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

	if (m_current_trap == TRAP_TYPE_INVALID)
		return;

	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_current_trap == TRAP_TYPE_INVALID)
		return;

	SGE_ASSERT(m_current_trap_L != NULL);

	m_current_trap_result = trap_be(m_current_trap_L, m_current_trap);

	m_current_trap = TRAP_TYPE_INVALID;
	m_current_trap_L = NULL;

	m_cond.notify_one();
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

void game::gmain(void)
{
	luaL_openlibs(m_L);

	init_traps();

	m_input.init();

	uv_loop_init(&m_loop);
	uv_async_init(&m_loop, &m_quit_async, &game::quit_async);
	m_quit_async.data = this;
	m_running = true;

	if (!init_main_task()) {
		SGE_LOGE("failed to laod main task.\n");
		uv_loop_close(&m_loop);
		m_state = STATE_FATAL;
		return;
	}

	m_state = STATE_READY;

	while (m_running) {
		schedule();
		uv_run(&m_loop, UV_RUN_ONCE);
	}

	m_input.shutdown();

	uv_loop_close(&m_loop);
}

int game::pmain(lua_State *L)
{
	auto _this = (game *)lua_touserdata(L, 1);
	SGE_ASSERT(_this != NULL);
	_this->gmain();
}

void game::tmain(void)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	SGE_ASSERT(m_L == NULL);
	SGE_ASSERT(m_state == STATE_IDLE);

	m_state = STATE_INITIALIZING;

	m_L = luaL_newstate();
	if (m_L == NULL) {
		m_state = STATE_FATAL;
		return;
	}

	attach_to_lua(m_L);

	lua_pushcfunction(m_L, &game::pmain);
	lua_pushlightuserdata(m_L, this);

	int ret;

	ret = lua_pcall(m_L, 1, 1, 0);
	// TODO

	ret = lua_toboolean(m_L, -1);
	// TODO

	lua_close(m_L);
	m_L = NULL;

	m_state = STATE_ENDED;

	SGE_LOGI("game ended.\n");
}

void game::schedule(void)
{
	sge_list_node *node;
	task_t *T;
	lua_State *L;

	for (;;) {
		node = sge_list_del_head(&m_task_list);
		if (node == sge_list_knot(&m_task_list))
			break;
		T = SGE_MEMBEROF(node, task_t, node);
		L = task_to_lua(T);
		int ret = lua_resume(L, m_L, 0);
		if (ret != LUA_YIELD && ret != LUA_OK) {
			SGE_LOGD("LUA_ERROR: %s\n", luaL_checkstring(L, -1));
			// TODO
		}
	}
}

bool game::init_main_task(void)
{
	SGE_ASSERT(m_L != NULL);
	SGE_ASSERT(m_main_task == NULL);

	PHYSFS_File *fp = PHYSFS_openRead(SGE_GAME_MAIN_RC);
	if (fp == NULL) {
		SGE_LOGE("Failed to open '%s'.\n", SGE_GAME_MAIN_RC);
		return false;
	}

	PHYSFS_sint64 length = PHYSFS_fileLength(fp);
	if (length < 1) {
		SGE_LOGE("Invalid lua script '%s'.\n", SGE_GAME_MAIN_RC);
		return false;
	}

	if (length > (16 * 1024 * 1024)) {
		SGE_LOGE("The size of '%s' is too big.\n", SGE_GAME_MAIN_RC);
		PHYSFS_close(fp);
		return false;
	}

	char *rc = (char *)malloc(length + 2);
	if (rc == NULL) {
		SGE_LOGE("Not enough memory for '%s'.\n", SGE_GAME_MAIN_RC);
		PHYSFS_close(fp);
		return false;
	}

	memset(rc, 0, length + 2);

	PHYSFS_sint64 readlen = PHYSFS_readBytes(fp, rc, length);
	if (readlen != length) {
		SGE_LOGE("Failed to load '%s'.\n", SGE_GAME_MAIN_RC);
		free(rc);
		PHYSFS_close(fp);
		return false;
	}

	lua_State *main_task = lua_newthread(m_L);
	if (main_task == NULL) {
		SGE_LOGE("Faile to create game main task.\n");
		free(rc);
		PHYSFS_close(fp);
		return false;
	}

	int ret = luaL_loadstring(main_task, rc);
	free(rc);
	PHYSFS_close(fp);

	if (ret) {
		SGE_LOGE("Failed to parse '%s'.\n", SGE_GAME_MAIN_RC);
		return false;
	}

	m_main_task = task_from_lua(main_task);

	return true;
}

void game::luaclose(lua_State *L)
{
}

void game::luathread(lua_State *L, lua_State *L1)
{
	attach_to_lua(L1);

	task_t *T = task_from_lua(L1);
	uv_timer_init(&m_loop, &T->sleep_timer);
	T->sleep_timer.data = T;

	sge_list_node_reset(&T->node);
	sge_list_add_tail(&m_task_list, &T->node);
}

void game::luafree(lua_State *L, lua_State *L1)
{
	SGE_ASSERT(this == from_lua(L));

	task_t *T = task_from_lua(L1);
	sge_list_node_unlink(&T->node);
}

void game::luaresume(lua_State *L, int n)
{
}

void game::luayield(lua_State *L, int n)
{
}

SGE_END

extern "C" void sge_game_luaopen(lua_State *L)
{
}

extern "C" void sge_game_luaclose(lua_State *L)
{
	sge::game::from_lua(L)->luaclose(L);
}

extern "C" void sge_game_luathread(lua_State *L, lua_State *L1)
{
	sge::game::from_lua(L)->luathread(L, L1);
}

extern "C" void sge_game_luafree(lua_State *L, lua_State *L1)
{
	sge::game::from_lua(L)->luafree(L, L1);
}

extern "C" void sge_game_luaresume(lua_State *L, int n)
{
	sge::game::from_lua(L)->luaresume(L, n);
}

extern "C" void sge_game_luayield(lua_State *L, int n)
{
	sge::game::from_lua(L)->luayield(L, n);
}

