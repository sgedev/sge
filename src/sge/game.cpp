//
//
#include <sge/game.hpp>

#define SGE_GAME_MAIN_RC "/init.lua"

#define SGE_GAME_TRAP_RESULT_NOT_SET	-1
#define SGE_GAME_TRAP_RESULT_NOT_IMPL	-2

SGE_BEGIN

static inline game::task_t *task_from_lua(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	return (game::task_t *)lua_getextraspace(L);
}

static inline lua_State *task_to_lua(game::task_t *T)
{
	SGE_ASSERT(T != NULL);

	return (lua_State *)SGE_PMOVB(T, sizeof(game::task_t));
}

static inline void game_attach_lua(game *G, lua_State *L)
{
	task_from_lua(L)->data = G;
}

static inline game *game_from_task(game::task_t *T)
{
	return (game *)(T->data);
}

static inline game *game_from_lua(lua_State *L)
{
	return (game *)(task_from_lua(L)->data);
}

game::game(void)
	: m_L(NULL)
	, m_state(STATE_IDLE)
	, m_current_trap(TRAP_INVALID)
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

	m_state = STATE_INITIALIZING;

	m_thread = std::thread(&game::tmain, this);
	if (!m_thread.joinable()) {
		m_scene.shutdown();
		return false;
	}

	sleep(1);

	SGE_LOGD("init waiting...\n");
	m_cond.wait(m_mutex);
	SGE_LOGD("init waiting done\n");

	if (m_state != STATE_READY) {
		if (m_thread.joinable())
			m_thread.join();
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

	SGE_LOGD("m_current_trap %d\n", m_current_trap);
	if (m_current_trap == TRAP_INVALID)
		return;

	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_current_trap == TRAP_INVALID)
		return;

	SGE_ASSERT(m_current_trap_L != NULL);

	m_current_trap_result = do_trap(m_current_trap_L, m_current_trap);
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
		m_cond.notify_one();
		return;
	}

	m_state = STATE_READY;
	SGE_LOGD("notify <<<<<<<<<<\n");
	m_cond.notify_all();

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
	SGE_LOGD("tmain locked.\n");

	SGE_ASSERT(m_L == NULL);
	SGE_ASSERT(m_state == STATE_INITIALIZING);

	m_L = luaL_newstate();
	if (m_L == NULL) {
		m_state = STATE_FATAL;
		m_cond.notify_one();
		return;
	}

	game_attach_lua(this, m_L);

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

void game::init_traps(void)
{
	lua_newtable(m_L);

	lua_pushcfunction(m_L, &game::on_trap_version);
	lua_setfield(m_L, -2, "version");

	lua_pushcfunction(m_L, &game::on_trap_task);
	lua_setfield(m_L, -2, "task");

	lua_pushcfunction(m_L, &game::on_trap_current);
	lua_setfield(m_L, -2, "current");

	lua_pushcfunction(m_L, &game::on_trap_sleep);
	lua_setfield(m_L, -2, "sleep");

	lua_pushcfunction(m_L, &game::on_trap_fps);
	lua_setfield(m_L, -2, "fps");

	lua_newtable(m_L);

	lua_pushcfunction(m_L, &game::on_trap_window_visibled);
	lua_setfield(m_L, -2, "visibled");

	lua_pushcfunction(m_L, &game::on_trap_window_show);
	lua_setfield(m_L, -2, "show");

	lua_pushcfunction(m_L, &game::on_trap_window_hide);
	lua_setfield(m_L, -2, "hide");

	lua_pushcfunction(m_L, &game::on_trap_window_title);
	lua_setfield(m_L, -2, "title");

	lua_pushcfunction(m_L, &game::on_trap_window_set_title);
	lua_setfield(m_L, -2, "set_title");

	lua_pushcfunction(m_L, &game::on_trap_window_position);
	lua_setfield(m_L, -2, "position");

	lua_pushcfunction(m_L, &game::on_trap_window_move);
	lua_setfield(m_L, -2, "move");

	lua_pushcfunction(m_L, &game::on_trap_window_size);
	lua_setfield(m_L, -2, "size");

	lua_pushcfunction(m_L, &game::on_trap_window_resize);
	lua_setfield(m_L, -2, "resize");

	lua_setfield(m_L, -2, "window");

	lua_newtable(m_L);

	lua_pushcfunction(m_L, &game::on_trap_editor_enabled);
	lua_setfield(m_L, -2, "enabled");

	lua_setfield(m_L, -2, "editor");

	lua_setglobal(m_L, "sge");
}

int game::on_trap(lua_State *L, trap_type tt)
{
	SGE_ASSERT(m_current_trap == TRAP_INVALID);
	SGE_ASSERT(m_current_trap_L == NULL);
	SGE_ASSERT(L != NULL);
	SGE_ASSERT(tt != TRAP_INVALID);

	m_current_trap = tt;
	m_current_trap_L = L;
	m_current_trap_result = SGE_GAME_TRAP_RESULT_NOT_SET;

	m_cond.wait(m_mutex);

	SGE_ASSERT(m_current_trap_result != SGE_GAME_TRAP_RESULT_NOT_SET);

	switch (m_current_trap_result) {
	case SGE_GAME_TRAP_RESULT_NOT_IMPL:
		luaL_error(L, "trap %d not impl.", tt);
		break;
	}

	return m_current_trap_result;
}

int game::do_trap(lua_State *L, trap_type tt)
{
	int ret;

	switch (tt) {
	case TRAP_FPS:
		ret = do_trap_fps(L);
		break;
	case TRAP_WINDOW_VISIBLED:
		ret = do_trap_window_visibled(L);
		break;
	case TRAP_WINDOW_SHOW:
		ret = do_trap_window_show(L);
		break;
	case TRAP_WINDOW_HIDE:
		ret = do_trap_window_hide(L);
		break;
	case TRAP_WINDOW_TITLE:
		ret = do_trap_window_title(L);
		break;
	case TRAP_WINDOW_SET_TITLE:
		ret = do_trap_window_set_title(L);
		break;
	case TRAP_WINDOW_POSITION:
		ret = do_trap_window_position(L);
		break;
	case TRAP_WINDOW_MOVE:
		ret = do_trap_window_move(L);
		break;
	case TRAP_WINDOW_SIZE:
		ret = do_trap_window_size(L);
		break;
	case TRAP_WINDOW_RESIZE:
		ret = do_trap_window_resize(L);
		break;
	case TRAP_EDITOR_ENABLED:
		ret = do_trap_editor_enabled(L);
		break;
	default:
		ret = -1;
		break;
	}

	return ret;
}

int game::on_trap_version(lua_State *L)
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

int game::on_trap_task(lua_State *L)
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

int game::on_trap_current(lua_State *L)
{
	if (L == game_from_lua(L)->m_L)
		luaL_error(L, "Not in a thread");

	lua_pushthread(L);

	return 1;
}

void game::on_trap_sleep_done(uv_timer_t *timer)
{
	task_t *T = (task_t *)(timer->data);
	SGE_ASSERT(T != NULL);

	game *G = game_from_task(T);
	SGE_ASSERT(G != NULL);
	SGE_ASSERT(sge_list_has(&G->m_task_list_sleep, &T->node));

	sge_list_del(&G->m_task_list_sleep, &T->node);
	sge_list_add_tail(&G->m_task_list, &T->node);
}

int game::on_trap_sleep(lua_State *L)
{
	SGE_LOGD("trap sleep...\n");

	int ms = luaL_checkinteger(L, 1);
	if (ms < 0)
		luaL_error(L, "bad arg");

	if (ms > 0) {
		task_t *T = task_from_lua(L);
		SGE_ASSERT(T != NULL);
		game *G = game_from_task(T);
		SGE_ASSERT(G != NULL);
		uv_timer_start(&T->sleep_timer, &game::on_trap_sleep_done, ms, 0);
		sge_list_add_tail(&G->m_task_list_sleep, &T->node);
	}

	return lua_yield(L, 0);
}

int game::on_trap_fps(lua_State *L)
{
	game *G = game_from_lua(L);
	SGE_ASSERT(G != NULL);
	return G->on_trap(L, TRAP_FPS);
}

int game::do_trap_fps(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_fps)
		return SGE_GAME_TRAP_RESULT_NOT_IMPL;

	lua_pushinteger(L, trap_fps());

	return 1;
}

int game::on_trap_window_visibled(lua_State *L)
{
	game *G = game_from_lua(L);
	SGE_ASSERT(G != NULL);
	return G->on_trap(L, TRAP_WINDOW_VISIBLED);
}

int game::do_trap_window_visibled(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_visibled)
		return SGE_GAME_TRAP_RESULT_NOT_IMPL;

	lua_pushboolean(L, trap_window_visibled());

	return 1;
}

int game::on_trap_window_show(lua_State *L)
{
	game *G = game_from_lua(L);
	SGE_ASSERT(G != NULL);
	return G->on_trap(L, TRAP_WINDOW_SHOW);
}

int game::do_trap_window_show(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_show)
		return SGE_GAME_TRAP_RESULT_NOT_IMPL;

	trap_window_show();

	return 0;
}

int game::on_trap_window_hide(lua_State *L)
{
	game *G = game_from_lua(L);
	SGE_ASSERT(G != NULL);
	return G->on_trap(L, TRAP_WINDOW_HIDE);
}

int game::do_trap_window_hide(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_hide)
		return SGE_GAME_TRAP_RESULT_NOT_IMPL;

	trap_window_hide();

	return 0;
}

int game::on_trap_window_title(lua_State *L)
{
	game *G = game_from_lua(L);
	SGE_ASSERT(G != NULL);
	return G->on_trap(L, TRAP_WINDOW_TITLE);
}

int game::do_trap_window_title(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_title)
		return SGE_GAME_TRAP_RESULT_NOT_IMPL;

	lua_pushstring(L, trap_window_title());

	return 1;
}

int game::on_trap_window_set_title(lua_State *L)
{
	game *G = game_from_lua(L);
	SGE_ASSERT(G != NULL);
	return G->on_trap(L, TRAP_WINDOW_SET_TITLE);
}

int game::do_trap_window_set_title(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_set_title)
		return SGE_GAME_TRAP_RESULT_NOT_IMPL;

	trap_window_set_title(luaL_checkstring(L, 1));

	return 0;
}

int game::on_trap_window_position(lua_State *L)
{
	game *G = game_from_lua(L);
	SGE_ASSERT(G != NULL);
	return G->on_trap(L, TRAP_WINDOW_POSITION);
}

int game::do_trap_window_position(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_position)
		return SGE_GAME_TRAP_RESULT_NOT_IMPL;

	int x, y;

	trap_window_position(x, y);

	lua_newtable(L);
	lua_pushinteger(L, x);
	lua_rawseti(L, -2, 0);
	lua_pushinteger(L, y);
	lua_rawseti(L, -2, 1);

	return 1;
}

int game::on_trap_window_move(lua_State *L)
{
	game *G = game_from_lua(L);
	SGE_ASSERT(G != NULL);
	return G->on_trap(L, TRAP_WINDOW_MOVE);
}

int game::do_trap_window_move(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_move)
		return SGE_GAME_TRAP_RESULT_NOT_IMPL;

	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);

	trap_window_move(x, y);

	return 0;
}

int game::on_trap_window_size(lua_State *L)
{
	game *G = game_from_lua(L);
	SGE_ASSERT(G != NULL);
	return G->on_trap(L, TRAP_WINDOW_SIZE);
}

int game::do_trap_window_size(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_size)
		return SGE_GAME_TRAP_RESULT_NOT_IMPL;

	int width, height;

	trap_window_size(width, height);

	lua_newtable(L);
	lua_pushinteger(L, width);
	lua_rawseti(L, -2, 0);
	lua_pushinteger(L, height);
	lua_rawseti(L, -2, 1);

	return 1;
}

int game::on_trap_window_resize(lua_State *L)
{
	game *G = game_from_lua(L);
	SGE_ASSERT(G != NULL);
	return G->on_trap(L, TRAP_WINDOW_RESIZE);
}

int game::do_trap_window_resize(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_resize)
		return SGE_GAME_TRAP_RESULT_NOT_IMPL;

	int width = luaL_checkinteger(L, 1);
	int height = luaL_checkinteger(L, 2);

	trap_window_resize(width, height);

	return 0;
}

int game::on_trap_editor_enabled(lua_State *L)
{
	game *G = game_from_lua(L);
	SGE_ASSERT(G != NULL);
	return G->do_trap(L, TRAP_EDITOR_ENABLED);
}

int game::do_trap_editor_enabled(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_editor_enabled)
		return SGE_GAME_TRAP_RESULT_NOT_IMPL;

	lua_pushboolean(L, trap_editor_enabled());

	return 1;
}

void game::on_luaclose(lua_State *L)
{
}

void game::on_luathread(lua_State *L, lua_State *L1)
{
	game_attach_lua(this, L1);

	task_t *T = task_from_lua(L1);

	uv_timer_init(&m_loop, &T->sleep_timer);
	T->sleep_timer.data = T;

	sge_list_node_reset(&T->node);
	sge_list_add_tail(&m_task_list, &T->node);
}

void game::on_luafree(lua_State *L, lua_State *L1)
{
	SGE_ASSERT(this == game_from_lua(L));

	task_t *T = task_from_lua(L);
	sge_list_node_unlink(&T->node);
}

void game::on_luaresume(lua_State *L, int n)
{
}

void game::on_luayield(lua_State *L, int n)
{
}

SGE_END

extern "C" void sge_game_luaopen(lua_State *L)
{
}

extern "C" void sge_game_luaclose(lua_State *L)
{
	sge::game_from_lua(L)->on_luaclose(L);
}

extern "C" void sge_game_luathread(lua_State *L, lua_State *L1)
{
	sge::game_from_lua(L)->on_luathread(L, L1);
}

extern "C" void sge_game_luafree(lua_State *L, lua_State *L1)
{
	sge::game_from_lua(L)->on_luafree(L, L1);
}

extern "C" void sge_game_luaresume(lua_State *L, int n)
{
	sge::game_from_lua(L)->on_luaresume(L, n);
}

extern "C" void sge_game_luayield(lua_State *L, int n)
{
	sge::game_from_lua(L)->on_luayield(L, n);
}

