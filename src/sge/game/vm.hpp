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


class vm: public noncopyable {
public:
	vm(void);
	virtual ~vm(void);

public:
	bool start(const char *initrc);
	void stop(void);
	void update(void);

public: // called by lua.
	void add_task(lua_State *L);
	void remove_task(lua_State *L);
	void resume_task(lua_State *L, int n);
	void yield_task(lua_State *L, int n);

private:
	void tmain(std::promise<bool> *init_result);
	static int pmain(lua_State *L);
	void vmain(std::promise<bool> *init_result);
	lua_State *load_main_task(void);
	void schedule_tasks(void);
	static void quit_cb(uv_async_t *async);
	void init_traps(void);

private: // traps
	static int trap_version(lua_State *L);
	static int trap_task(lua_State *L);
	static void sleep_done(uv_timer_t *timer);
	static int trap_sleep(lua_State *L);

private:
	lua_State *m_L;
	uv_loop_t m_loop;
	uv_async_t m_quit_async;
	bool m_running;
	xlist_t m_task_list;
	xlist_t m_task_list_sleep;
	std::thread m_thread;
	std::string m_init_script;
};

SGE_GAME_END

#endif // SGE_GAME_VM_HPP

