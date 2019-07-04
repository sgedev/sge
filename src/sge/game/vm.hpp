//
//
#ifndef SGE_GAME_VM_HPP
#define SGE_GAME_VM_HPP

#include <future>
#include <thread>
#include <string>

#include <lua.hpp>

#include <sge/noncopyable.hpp>
#include <sge/game/common.hpp>

SGE_GAME_BEGIN

typedef sge_game_vm_task_t task_t;

static inline task_t *task_from_lua(lua_State *L)
{
	return (task_t *)lua_getextraspace(L);
}

static inline lua_State *task_to_lua(task_t *task)
{
	return (lua_State *)(((char *)task) + LUA_EXTRASPACE);
}

class vm;

static inline void task_set_vm(task_t *task, vm *pvm)
{
	task->data = pvm;
}

static inline void lua_set_vm(lua_State *L, vm *pvm)
{
	task_set_vm(task_from_lua(L), pvm);
}

static inline vm *vm_from_task(task_t *task)
{
	return (vm *)(task->data);
}

static inline vm *vm_from_lua(lua_State *L)
{
	return vm_from_task(task_from_lua(L));
}

class vm: public noncopyable {
public:
	enum {
		STATE_IDLE = 0,
		STATE_INITIALING,
		STATE_RUNNING,
		STATE_EXITING,
		STATE_EXITED
	};

public:
	vm(void);
	virtual ~vm(void);

public:
	bool start(const char *initrc);
	void stop(void);
	void update(void);
	int state(void) const;

public: // called by lua.
	void add_task(lua_State *L);
	void remove_task(lua_State *L);
	void resume_task(lua_State *L, int n);
	void yield_task(lua_State *L, int n);

private:
	void tmain(std::promise<bool> *init_result);
	static int pmain(lua_State *L);
	void vmain(std::promise<bool> *init_result);
	bool init_main_task(void);
	void schedule_tasks(void);
	static void quit_cb(uv_async_t *async);
	void init_traps(void);

private: // traps
	static int trap_version(lua_State *L);
	static int trap_task(lua_State *L);
	static int trap_current(lua_State *L);
	static int trap_join(lua_State *L);
	static void sleep_done(uv_timer_t *timer);
	static int trap_sleep(lua_State *L);
	static int trap_execute(lua_State *L);
	static int trap_self(lua_State *L);
	static int trap_wait(lua_State *L);

private:
	void unlink(void);
	void set_parent(vm *parent);

private:
	lua_State *m_L;
	uv_loop_t m_loop;
	uv_async_t m_quit_async;
	xlist_t m_task_list;
	xlist_t m_task_list_sleep;
	std::thread m_thread;
	std::string m_initrc;
	task_t *m_main_task;
	int m_state;

private:
	vm *m_parent;
	vm *m_prev_sibling;
	vm *m_next_sibling;
	vm *m_first_child;

	task_t *m_join_task;
	xlist_t m_join_task_list;

	task_t *m_wait_task;
	xlist_t m_wait_task_list;
};

inline int vm::state(void) const
{
	return m_state;
}

SGE_GAME_END

#endif // SGE_GAME_VM_HPP

