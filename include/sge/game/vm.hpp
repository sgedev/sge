//
//
#ifndef SGE_GAME_VM_HPP
#define SGE_GAME_VM_HPP

#include <future>
#include <thread>

#include <lua.hpp>

#include <sge/noncopyable.hpp>
#include <sge/signal.hpp>
#include <sge/db.hpp>
#include <sge/game/common.hpp>

SGE_GAME_BEGIN

class vm: public noncopyable {
public:
	vm(void);
	virtual ~vm(void);

public:
	signal<int> exited;

public:
	bool start(db::blob_ptr initrc);
	void stop(void);
	void poll(void);

public: // called by lua.
	static void init(lua_State *L);
	static void shutdown(lua_State *L);
	static void add_task(lua_State *L, lua_State *L1);
	static void remove_task(lua_State *L, lua_State *L1);
	static void resume_task(lua_State *L, int n);
	static void yield_task(lua_State *L, int n);

private:
	typedef sge_game_vm_task_t task_t;

	void schedule_tasks(void);
	void init_traps(void);
	static void quit_cb(uv_async_t *async);
	void lmain(std::promise<bool> *init_result);
	static int pmain(lua_State *L);
	void tmain(std::promise<bool> *init_result);
	void unlink(void);
	static task_t *task_from_lua(lua_State *L);
	static lua_State *task_to_lua(task_t *task);
	void bind_lua(lua_State *L);
	static vm *from_lua(lua_State *L);

private:
	static int trap_version(lua_State *L);
	static int trap_task(lua_State *L);
	static int trap_exec(lua_State *L);
	static int trap_kill(lua_State *L);
	static int trap_sleep(lua_State *L);

private:
	uv_loop_t *m_loop;
	uv_loop_t *m_thread_loop;
	std::thread m_thread;
	lua_State *m_L;
	uv_async_t m_quit_async;
	db::blob_ptr m_initrc;
	bool m_running;
	xlist_t m_task_list;
	vm *m_parent;
	vm *m_prev;
	vm *m_next;
	vm *m_first_child;
};

inline vm::task_t *vm::task_from_lua(lua_State *L)
{
	return (vm::task_t *)(lua_getextraspace(L));
}

inline lua_State *vm::task_to_lua(task_t *task)
{
	return (lua_State *)SGE_PMOVB(task, LUA_EXTRASPACE);
}

inline void vm::bind_lua(lua_State *L)
{
	task_from_lua(L)->vm = (void *)this;
}

inline vm *vm::from_lua(lua_State *L)
{
	return (vm *)(task_from_lua(L)->vm);
}

SGE_GAME_END

#endif // SGE_GAME_VM_HPP

