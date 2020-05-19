//
//
#ifndef SGE_VM_HPP
#define SGE_VM_HPP

#include <string>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

#include <lua.hpp>
#include <cx/list.h>

#include <SGE/Common.hpp>

SGE_BEGIN

class VM {
public:
	VM(void);
	virtual ~VM(void);

public:
	const std::string &root(void) const;
	virtual bool start(const std::string &initrc);
	virtual void stop(void);
	void poll(void);

public: // for lua
	typedef SGEVMTask Task;
	static Task *taskFromLua(lua_State *L);
	static lua_State *taskToLua(Task *T);
	static VM *fromTask(Task *T);
	static VM *fromLua(lua_State *L);
	void taskAddedHook(lua_State *L, lua_State *L1);
	void taskRemovedHook(lua_State *L, lua_State *L1);
	void taskResumeHook(lua_State *L, int n);
	void taskYieldHook(lua_State *L, int n);

protected:
	virtual int trapFps(void);

private:
	void threadMain(const std::string &initrc, std::promise<bool> *initResult);
	static int luaMain(lua_State *L);
	void vmMain(lua_State *L, const char *initrc, std::promise<bool> &initResult);
	static void asyncExit(uv_async_t *p);
	void initExports(lua_State *L);
	bool loadInitTask(lua_State *L, const char *initrc);
	void schedule(lua_State *L);

private:
	enum Trap {
		TrapInvalid = 0,
		TrapFps,
		TrapMount
	};

	enum TrapError {
		TrapNosys = -1
	};

	int doTrap(lua_State *L, Trap trap);

	static int luaFps(lua_State *L);
	int luaFpsTrap(lua_State *L);

	static int luaMount(lua_State *L);
	int luaMountTrap(lua_State *L);

	static int luaTask(lua_State *L);

	static void luaSleepDone(uv_timer_t *p);
	static int luaSleep(lua_State *L);

private:
	std::string m_root;
	bool m_run;
	uv_async_t m_asyncExit;
	std::mutex m_mutex;
	std::condition_variable_any m_cond;
	std::thread m_thread;
	uv_loop_t m_loop;
	cx_list_t m_taskListReady;
	cx_list_t m_taskListSleep;
	Task *m_current;
	Trap m_trap;
	lua_State *m_trapStack;
	int m_trapResult;
};

SGE_INLINE const std::string &VM::root(void) const
{
	return m_root;
}

SGE_INLINE VM::Task *VM::taskFromLua(lua_State *L)
{
	return (Task *)lua_getextraspace(L);
}

SGE_INLINE lua_State *VM::taskToLua(Task *T)
{
	return (lua_State *)CX_PMOVB(T, LUA_EXTRASPACE);
}

SGE_INLINE VM *VM::fromTask(Task *T)
{
	return (VM *)(T->vm);
}

SGE_INLINE VM *VM::fromLua(lua_State *L)
{
	return fromTask(taskFromLua(L));
}

SGE_END

#endif // SGE_VM_HPP
