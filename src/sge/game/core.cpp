//
//
#include <future>

#include <lua.hpp>

#include <sge/db.hpp>
#include <sge/game.hpp>

SGE_GAME_BEGIN

typedef sge_game_task_t task_t;

static lua_State *s_lua;
static uv_loop_t *s_loop;
static uv_async_t s_quit_async;
static bool s_running;
static xlist_t s_task_list;
static xlist_t s_task_list_sleep;
static std::thread s_thread;

static inline task_t *task_from_lua(lua_State *L)
{
	SGE_ASSERT(L != s_lua);
	return (task_t *)lua_getextraspace(L);
}

static inline lua_State *task_to_lua(task_t *task)
{
	return (lua_State *)SGE_PMOVB(task, LUA_EXTRASPACE);
}

static int trap_print(lua_State *L)
{
	int n = lua_gettop(L);
	int i;

	printf("LUA: ");

	lua_getglobal(L, "tostring");

	for (i = 1; i <= n; i++) {
		const char *s;
		size_t l;
		lua_pushvalue(L, -1);
		lua_pushvalue(L, i);
		lua_call(L, 1, 1);
		s = lua_tolstring(L, -1, &l);
		if (s == NULL)
			return luaL_error(L, "'tostring' must return a string to 'print'");
		printf("%.*s\n", (int)l, s);
		lua_pop(L, 1);
	}

	return 0;
}

static int trap_version(lua_State *L)
{
	lua_newtable(L);
	lua_pushinteger(L, SGE_VERSION_MAJOR);
	lua_setfield(L, -2, "major");
	lua_pushinteger(L, SGE_VERSION_MINOR);
	lua_setfield(L, -2, "minor");
	lua_pushinteger(L, SGE_VERSION_PATCH);
	lua_setfield(L, -2, "patch");

	return 1;
}

static int trap_task(lua_State *L)
{
	luaL_argcheck(L, lua_isfunction(L, 1), 1, "not function");

	lua_State *L1 = lua_newthread(L);
	if (L1 == NULL) {
		lua_pushnil(L);
		return 1;
	}

	lua_pushvalue(L, 1);
	lua_xmove(L, L1, 1);

	uv_stop(s_loop);

	return 1;
}

static void sleep_done(uv_timer_t *timer)
{
	task_t *T = (task_t *)(timer->data);
	SGE_ASSERT(T != NULL);
	SGE_ASSERT(xlist_has(&s_task_list_sleep, &T->node));

	xlist_del(&s_task_list_sleep, &T->node);
	xlist_add_tail(&s_task_list, &T->node);

	/* uv_stop(s_loop); */
}

static int trap_sleep(lua_State *L)
{
	SGE_LOGD("trap sleep...\n");

	int ms = luaL_checkinteger(L, 1);
	if (ms < 0)
		luaL_error(L, "bad arg");

	if (ms > 0) {
		task_t *T = task_from_lua(L);
		uv_timer_start(&T->sleep_timer, sleep_done, ms, 0);
		xlist_add_tail(&s_task_list_sleep, &T->node);
	}

	return lua_yield(L, 0);
}

static void init_traps(void)
{
	SGE_ASSERT(s_lua != NULL);

	luaL_openlibs(s_lua);

	lua_pushcfunction(s_lua, &trap_print);
	lua_setglobal(s_lua, "print");

	lua_newtable(s_lua);

	lua_pushcfunction(s_lua, &trap_version);
	lua_setfield(s_lua, -2, "version");

	lua_pushcfunction(s_lua, &trap_task);
	lua_setfield(s_lua, -2, "task");

	lua_pushcfunction(s_lua, &trap_sleep);
	lua_setfield(s_lua, -2, "sleep");

	lua_setglobal(s_lua, "sge");
}

static inline void open(lua_State *L)
{
	SGE_ASSERT(s_lua == NULL);
	SGE_ASSERT(L != NULL);
	s_lua = L;
	xlist_reset(&s_task_list);
	init_traps();

	SGE_LOGD("main lua %p\n", L);
}

static inline void close(lua_State *L)
{
	SGE_ASSERT(s_lua != NULL);
	s_lua = NULL;

	SGE_LOGD("lua closed.\n");
}

static inline void thread(lua_State *L, lua_State *L1)
{
	SGE_ASSERT(L == s_lua);
	SGE_ASSERT(s_loop != NULL);

	SGE_LOGD("new task %p\n", L1);

	task_t *T = task_from_lua(L1);
	uv_timer_init(s_loop, &T->sleep_timer);
	T->sleep_timer.data = T;

	xlist_add_tail(&s_task_list, &(T->node));
}

static inline void free(lua_State *L, lua_State *L1)
{
	SGE_LOGD("free task %p\n", L1);

	SGE_ASSERT(L == s_lua);

	xlist_node_t *node = &(task_from_lua(L1)->node);
	SGE_ASSERT(node != NULL);

	xlist_node_unlink(node);
}

static inline void resume(lua_State *L, int n)
{
	SGE_LOGD("resume task %p\n", L);

	if (L == s_lua)
		luaL_error(s_lua, "yield main task");

	xlist_node_t *node = &(task_from_lua(L)->node);
	SGE_ASSERT(node != NULL);
}

static inline void yield(lua_State *L, int n)
{
	SGE_LOGD("yield task %p\n", L);

	if (L == s_lua)
		luaL_error(s_lua, "yield main task");

	xlist_node_t *node = &(task_from_lua(L)->node);
	SGE_ASSERT(node != NULL);

	// if this task is not in any waiting list,
	// we put it back to run list tail.
	if (!xlist_node_linked(node))
		xlist_add_tail(&s_task_list, node);
}

static void schedule(void)
{
	xlist_node_t *node;
	task_t *T;
	lua_State *L;

	for (;;) {
		node = xlist_del_head(&s_task_list);
		if (node == xlist_knot(&s_task_list))
			break;
		T = SGE_MEMBEROF(node, task_t, node);
		L = task_to_lua(T);
		int ret = lua_resume(L, s_lua, 0);
		if (ret != LUA_YIELD && ret != LUA_OK) {
			SGE_LOGD("LUA_ERROR: %s\n", luaL_checkstring(L, -1));
			// TODO
		}
	}
}

static void quit_cb(uv_async_t *async)
{
	s_running = false;
}

static int pmain(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	std::promise<bool> *init_result = (std::promise<bool> *)lua_touserdata(L, 1);
	SGE_ASSERT(init_result != NULL);

	SGE_ASSERT(s_lua == L);
	SGE_ASSERT(s_loop == NULL);
	SGE_ASSERT(init_result != NULL);

	SGE_LOGI("Loading game...\n");

	db::node init_node = db::get("/init");
	if (!init_node) {
		SGE_LOGE("Failed to get /init.\n");
		init_result->set_value(false);
		return 0;
	}

	db::blob_ptr initrc = init_node.to_blob();
	if (!initrc) {
		SGE_LOGE("Failed to get /init content.\n");
		init_result->set_value(false);
		return 0;
	}

	s_loop = uv_loop_new();
	if (s_loop == NULL) {
		SGE_LOGE("Failed to create game main loop object.\n");
		init_result->set_value(false);
		return 0;
	}

	lua_State *main_task = lua_newthread(s_lua);
	if (main_task == NULL) {
		SGE_LOGE("Faile to create game main task.\n");
		init_result->set_value(false);
		uv_loop_delete(s_loop);
		s_loop = NULL;
	}

	initrc->open();
	int rc = luaL_loadstring(main_task, (const char *)(initrc->get_data()));
	initrc->close();

	if (rc) {
		SGE_LOGE("Failed to load initrc.\n");
		init_result->set_value(false);
		uv_loop_delete(s_loop);
		s_loop = NULL;
		return 0;
	}

	s_running = true;
	uv_async_init(s_loop, &s_quit_async, quit_cb);

	SGE_LOGI("Game running...\n");
	init_result->set_value(true);

	while (s_running) {
		schedule();
		uv_run(s_loop, UV_RUN_ONCE);
	}

	uv_loop_delete(s_loop);
	s_loop = NULL;

	SGE_LOGI("Game exited.\n");

	return 0;
}

static void main(std::promise<bool> *init_result)
{
	SGE_ASSERT(init_result != NULL);

	lua_State *L = luaL_newstate();
	if (L == NULL) {
		SGE_LOGE("cannot create state: not enough memory\n");
		init_result->set_value(false);
		return;
	}

	lua_pushcfunction(L, &pmain);
	lua_pushlightuserdata(L, init_result);
	int status = lua_pcall(L, 1, 0, 0);
	lua_close(L);

	if (status != LUA_OK) {
		// TODO
	}
}

bool init(void)
{
	s_running = false;

	xlist_reset(&s_task_list);
	xlist_reset(&s_task_list_sleep);

	std::promise<bool> init_result;
	auto init_result_future = init_result.get_future();

	// start game thread.
	s_thread = std::thread(main, &init_result);

	init_result_future.wait();
	if (init_result_future.get())
		return true;

	if (s_thread.joinable())
		s_thread.join();

	return false;
}

void shutdown(void)
{
	if (s_running) {
		uv_async_send(&s_quit_async);
		s_thread.join();
	}
}

void update(float elapsed)
{
}

bool can_quit(void)
{
	return true;
}

SGE_GAME_END

extern "C" void sge_game_open(lua_State *L)
{
	sge::game::open(L);
}

extern "C" void sge_game_close(lua_State *L)
{
	sge::game::close(L);
}

extern "C" void sge_game_thread(lua_State *L, lua_State *L1)
{
	sge::game::thread(L, L1);
}

extern "C" void sge_game_free(lua_State *L, lua_State *L1)
{
	sge::game::free(L, L1);
}

extern "C" void sge_game_resume(lua_State *L, int n)
{
	sge::game::resume(L, n);
}

extern "C" void sge_game_yield(lua_State *L, int n)
{
	sge::game::yield(L, n);
}

