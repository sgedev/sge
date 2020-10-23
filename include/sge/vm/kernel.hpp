//
//
#ifndef SGE_VM_KERNEL_HPP
#define SGE_VM_KERNEL_HPP

#include <sge/vm/common.hpp>
#include <sge/vm/core.hpp>

SGE_VM_BEGIN

class kernel: public core {
public:
	kernel(uv_loop_t *loop);
	virtual ~kernel(void);

public:
    bool start(const std::string &rootfs, const std::string &initrc) override;
    void stop(void) override;

public: // for lua
	static void task_added(lua_State *L, lua_State *L1);
	static void task_removed(lua_State *L, lua_State *L1);
	static void task_resume(lua_State *L, int n);
	static void task_yield(lua_State *L, int n);

protected:
    typedef sge_vm_task_t task_t;

    enum {
        TRAP_TYPE_INVALID = -1,
        TRAP_TYPE_FOO = 0,
        TRAP_TYPE_USER
    };

    enum {
        TRAP_ERROR_NOT_SET = -1,
        TRAP_ERROR_NOT_IMPL = -2
    };

    static task_t *task_from_lua(lua_State *L);
    static lua_State *task_to_lua(task_t *task);
    static kernel *from_task(task_t *task);
    static kernel *from_lua(lua_State *L);

    void run(uv_loop_t *loop) override;
	void handle_event(const SDL_Event &evt) override;
    virtual void init_lua(lua_State *L);
    virtual void trap_foo(void) = 0;

private:
	void kmain(lua_State *L, uv_loop_t *loop);
	bool load_initrc(lua_State *L);
	static void schedule(uv_prepare_t *p);

private:
    static void handle_traps(uv_async_t *p);
    static int trap_done(lua_State *L, int status, lua_KContext ctx);
    int do_trap(lua_State *L, int type);

    static int handle_trap_foo(lua_State *L);

private:
	// sge.current
    static int sys_current(lua_State *L);

	// sge.task
    static int sys_task(lua_State *L);

	// sge.sleep
    static void sys_sleep_done(uv_timer_t *p);
    static int sys_sleep(lua_State *L);

	// sge.wait
    static int sys_wait(lua_State *L);

private:
    spin_lock m_task_mutex;
	sge_list_t m_task_list;
    sge_list_t m_task_list_trap;
    uv_async_t m_trap_async;
};

SGE_INLINE kernel::task_t *kernel::task_from_lua(lua_State *L)
{
	SGE_ASSERT(L != nullptr);
	return (task_t *)lua_getextraspace(L);
}

SGE_INLINE lua_State *kernel::task_to_lua(task_t *task)
{
	SGE_ASSERT(task != nullptr);
	return (lua_State *)SGE_PMOVB(task, LUA_EXTRASPACE);
}

SGE_INLINE kernel *kernel::from_task(task_t *task)
{
	SGE_ASSERT(task != nullptr);
    return reinterpret_cast<kernel *>(task->kernel);
}

SGE_INLINE kernel *kernel::from_lua(lua_State *L)
{
	SGE_ASSERT(L != nullptr);
    return reinterpret_cast<kernel *>(task_from_lua(L)->kernel);
}

SGE_VM_END

#endif // SGE_VM_KERNEL_HPP
