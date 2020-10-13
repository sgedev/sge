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

public: // for lua
	static void task_added(lua_State *L, lua_State *L1);
	static void task_removed(lua_State *L, lua_State *L1);
	static void task_resume(lua_State *L, int n);
	static void task_yield(lua_State *L, int n);

protected:
	void run(uv_loop_t *loop) override;
	void handle_event(const SDL_Event &evt) override;

protected:
	typedef sge_vm_task_t task_t;

	static task_t *task_from_lua(lua_State *L);
	static lua_State *task_to_lua(task_t *task);
	static kernel *from_task(task_t *task);
	static kernel *from_lua(lua_State *L);

private:
	void kmain(lua_State *L, uv_loop_t *loop);
	bool load_initrc(lua_State *L);
	static void schedule(uv_prepare_t *p);

private:
	void init_exports(lua_State *L);

	// sge.current
	static int trap_current(lua_State *L);

	// sge.task
	static int trap_task(lua_State *L);

	// sge.sleep
	static void trap_sleep_done(uv_timer_t *p);
	static int trap_sleep(lua_State *L);

	// sge.wait
	static int trap_wait(lua_State *L);

private:
	sge_list_t m_task_list;
	uv_prepare_t m_schedule_preparer;
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
	return reinterpret_cast<kernel *>(task->data);
}

SGE_INLINE kernel *kernel::from_lua(lua_State *L)
{
	SGE_ASSERT(L != nullptr);
	return reinterpret_cast<kernel *>(task_from_lua(L)->data);
}

SGE_VM_END

#endif // SGE_VM_KERNEL_HPP
