//
//
#include <future>
#include <thread>

#include <lua.hpp>

#include <sge/db.hpp>
#include <sge/input.hpp>
#include <sge/scene.hpp>
#include <sge/game.hpp>

SGE_GAME_BEGIN

static db::node s_node;
static std::thread s_vm_thread;

namespace vm {
	static lua_State *s_lua;
	static uv_loop_t *s_loop;
	static uv_async_t s_quit_async;
	static db::blob_ptr s_initrc;
	static bool s_running;
	static xlist_t s_thread_list;

	static inline xlist_node_t *thread_node(lua_State *L)
	{
		SGE_ASSERT(L != s_lua);
		return (xlist_node_t *)lua_getextraspace(L);
	}

	static inline lua_State *thread_node_to_lua(xlist_node_t *node)
	{
		return (lua_State *)SGE_PMOVB(node, LUA_EXTRASPACE);
	}

	static void init_traps(void)
	{
		SGE_ASSERT(s_lua != NULL);

		luaL_openlibs(s_lua);

		lua_newtable(s_lua);

#if 0
		lua_pushcfunction(s_lua, &vm::trap_version);
		lua_setfield(s_lua, -2, "version");

		lua_pushcfunction(s_lua, &vm::trap_task);
		lua_setfield(s_lua, -2, "task");

		lua_pushcfunction(s_lua, &vm::trap_exec);
		lua_setfield(s_lua, -2, "exec");

		lua_pushcfunction(s_lua, &vm::trap_kill);
		lua_setfield(s_lua, -2, "kill");

		lua_pushcfunction(s_lua, &vm::trap_sleep);
		lua_setfield(s_lua, -2, "sleep");
#endif

		lua_setglobal(s_lua, "sge");
	}

	static inline void open(lua_State *L)
	{
		SGE_ASSERT(s_lua == NULL);
		SGE_ASSERT(L != NULL);
		s_lua = L;
		xlist_reset(&s_thread_list);
		init_traps();
	}

	static inline void close(lua_State *L)
	{
		SGE_ASSERT(s_lua != NULL);
		s_lua = NULL;
	}

	static inline void thread(lua_State *L, lua_State *L1)
	{
		SGE_ASSERT(L == s_lua);
		xlist_add_tail(&s_thread_list, thread_node(L1));
	}

	static inline void free(lua_State *L, lua_State *L1)
	{
		SGE_ASSERT(L == s_lua);
		xlist_node_unlink(thread_node(L1));
	}

	static inline void resume(lua_State *L, int n)
	{
	}

	static inline void yield(lua_State *L, int n)
	{
	}

	static void schedule(void)
	{
		xlist_node_t *node;
		lua_State *L;

		for (;;) {
			node = xlist_del_head(&s_thread_list);
			if (node == NULL)
				break;
			L = thread_node_to_lua(node);
			SGE_ASSERT(L != NULL);
			int ret = lua_resume(s_lua, L, 0);
			switch (ret) {
			case LUA_YIELD:
				if (!xlist_node_linked(node))
					xlist_add_tail(&s_thread_list, node);
				break;
			case LUA_OK:
				break;
			default:
				luaL_error(s_lua, "fatal error");
				break;
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
		SGE_ASSERT(s_initrc);
		SGE_ASSERT(init_result != NULL);

		s_loop = uv_loop_new();
		if (s_loop == NULL) {
			init_result->set_value(false);
			return 0;
		}

		if (!s_initrc->open()) {
			init_result->set_value(false);
			uv_loop_delete(s_loop);
			s_loop = NULL;
			return 0;
		}

		lua_State *main_task = lua_newthread(s_lua);
		if (main_task == NULL) {
			init_result->set_value(false);
			uv_loop_delete(s_loop);
			s_loop = NULL;
		}

		int rc = luaL_loadstring(main_task, (const char *)s_initrc->get_data());
		s_initrc->close();

		if (rc) {
			init_result->set_value(false);
			uv_loop_delete(s_loop);
			s_loop = NULL;
			return 0;
		}

		s_running = true;
		init_result->set_value(true);

		uv_async_init(s_loop, &s_quit_async, quit_cb);

		while (s_running) {
			schedule();
			uv_run(s_loop, UV_RUN_ONCE);
		}

		uv_loop_delete(s_loop);
		s_loop = NULL;

		return 0;
	}

	static void main(db::blob_ptr init_script, std::promise<bool> *init_result)
	{
		SGE_ASSERT(init_result != NULL);

		lua_State *L = luaL_newstate();
		if (L == NULL) {
			SGE_LOGE("cannot create state: not enough memory\n");
			init_result->set_value(false);
			return;
		}

		s_initrc = init_script;

		lua_pushcfunction(L, &vm::pmain);
		lua_pushlightuserdata(L, init_result);
		int status = lua_pcall(L, 1, 0, 0);
		lua_close(L);

		if (status != LUA_OK) {
			// TODO
		}
	}
}

bool init(void)
{
	db::node node = db::get("/init");
	if (!node)
		return false;

	db::blob_ptr init_script = node.to_blob();
	if (!init_script)
		return false;

	std::promise<bool> init_result;
	auto init_result_future = init_result.get_future();

	s_vm_thread = std::thread(vm::main, init_script, &init_result);

	init_result_future.wait();
	if (!init_result_future.get())
		return false;

	s_node = node;

	return true;
}

void shutdown(void)
{
}

void update(float elapsed)
{
}

bool can_quit(void)
{
	return true;
}

SGE_GAME_END

extern "C" void sge_game_vm_open(lua_State *L)
{
	sge::game::vm::open(L);
}

extern "C" void sge_game_vm_close(lua_State *L)
{
	sge::game::vm::close(L);
}

extern "C" void sge_game_vm_thread(lua_State *L, lua_State *L1)
{
	sge::game::vm::thread(L, L1);
}

extern "C" void sge_game_vm_free(lua_State *L, lua_State *L1)
{
	sge::game::vm::free(L, L1);
}

extern "C" void sge_game_vm_resume(lua_State *L, int n)
{
	sge::game::vm::resume(L, n);
}

extern "C" void sge_game_vm_yield(lua_State *L, int n)
{
	sge::game::vm::yield(L, n);
}

