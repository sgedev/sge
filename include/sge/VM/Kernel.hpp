//
//
#ifndef SGE_VM_KERNEL_HPP
#define SGE_VM_KERNEL_HPP

#include <array>
#include <string>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

#include <lua.hpp>
#include <cx/list.h>

#include <SGE/VM/Common.hpp>
#include <SGE/VM/Root.hpp>

SGE_VM_BEGIN

class Kernel {
public:
	Kernel(uv_loop_t *trapLoop);
	virtual ~Kernel(void);

public:
	uv_loop_t *trapLoop(void);
	Root &root(void);
	virtual bool start(const std::string &initrc);
	virtual void stop(void);

public: // for lua
	typedef SGEVMTask Task;
	static Task *taskFromLua(lua_State *L);
	static lua_State *taskToLua(Task *T);
	static Kernel *fromTask(Task *T);
	static Kernel *fromLua(lua_State *L);
	void taskAddedHook(lua_State *L, lua_State *L1);
	void taskRemovedHook(lua_State *L, lua_State *L1);
	void taskResumeHook(lua_State *L, int n);
	void taskYieldHook(lua_State *L, int n);

protected:
	bool inKernel(void) const;
	lua_State *lua(void);
	uv_loop_t *loop(void);
	virtual bool init(void) = 0;
	virtual void shutdown(void) = 0;

private:
	void startup(const std::string &initrc, std::promise<bool> *initResult);
	static int pmain(lua_State *L);
	void kmain(lua_State *L, const char *initrc, std::promise<bool> &initResult);
	void initExports(lua_State *L);
	bool loadInitTask(lua_State *L, const char *initrc);
	void schedule(lua_State *L);
	static void exitAsync(uv_async_t *p);

private:
	enum TrapType {
		TrapTypeInvalid = 0
	};

	enum TrapError {
		TrapErrorNotImpl = -1,
		TrapErrorUnknown = -2
	};

	void trapsHandler(uv_loop_t *loop);
	static void trapAsync(uv_async_t *p);
	static int trapDone(lua_State *L, int status, lua_KContext ctx);
	int trap(lua_State *L, TrapType type);

	static int luaMount(lua_State *L);

	static int luaTask(lua_State *L);

	static void luaSleepDone(uv_timer_t *p);
	static int luaSleep(lua_State *L);

private:
	uv_loop_t *m_trapLoop;
	bool m_run;
	uv_async_t m_exitAsync;
	uv_async_t m_trapAsync;
	std::mutex m_mutex;
	std::condition_variable_any m_cond;
	std::thread m_thread;

private:
	lua_State *m_L;
	uv_loop_t m_loop;
	Root m_root;
	cx_list_t m_taskListReady;
	cx_list_t m_taskListSleep;
	cx_list_t m_taskListTraps;
};

SGE_INLINE uv_loop_t *Kernel::trapLoop(void)
{
	return m_trapLoop;
}

SGE_INLINE Root &Kernel::root(void)
{
	SGE_ASSERT(inKernel());
	return m_root;
}

SGE_INLINE Kernel::Task *Kernel::taskFromLua(lua_State *L)
{
	return (Task *)lua_getextraspace(L);
}

SGE_INLINE lua_State *Kernel::taskToLua(Task *T)
{
	return (lua_State *)CX_PMOVB(T, LUA_EXTRASPACE);
}

SGE_INLINE Kernel *Kernel::fromTask(Task *T)
{
	return (Kernel *)(T->K);
}

SGE_INLINE Kernel *Kernel::fromLua(lua_State *L)
{
	return fromTask(taskFromLua(L));
}

SGE_INLINE bool Kernel::inKernel(void) const
{
	return (std::this_thread::get_id() == m_thread.get_id());
}

SGE_INLINE lua_State *Kernel::lua(void)
{
	SGE_ASSERT(inKernel());
	return m_L;
}

SGE_INLINE uv_loop_t *Kernel::loop(void)
{
	SGE_ASSERT(inKernel());
	return &m_loop;
}

SGE_VM_END

#endif // SGE_VM_KERNEL_HPP
