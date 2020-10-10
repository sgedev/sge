//
//
#ifndef SGE_VM_KERNEL_HPP
#define SGE_VM_KERNEL_HPP

#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <sge/gui/desktop.hpp>
#include <sge/scene/world.hpp>

#include <sge/vm/common.hpp>

SGE_VM_BEGIN

typedef sge_vm_task_t task_t;

class kernel {
public:
	typedef enum {
		STATE_NOT_START = 0,
		STATE_RUNNING,
		STATE_EXITING,
		STATE_ERROR
	} state_t;

public:
	kernel(uv_loop_t *loop);
	virtual ~kernel(void);

public:
	bool start(const std::string &rootfs, const std::string &initrc);
	void stop(void);
	void handle_event(const SDL_Event &evt);
	const std::string &rootfs(void) const;
	const std::string &initrc(void) const;

public: // for lua
	static void task_added(lua_State *L, lua_State *L1);
	static void task_removed(lua_State *L, lua_State *L1);
	static void task_resume(lua_State *L, int n);
	static void task_yield(lua_State *L, int n);

private:
	void set_state(state_t st);
	void host_run(std::function<void (void)> f);
	static void host(uv_async_t *p);
	static void quit(uv_async_t *p);
	void kmain(lua_State *L);
	static int pmain(lua_State *L);
	bool init_rootfs(void);
	bool load_initrc(lua_State *L);
	void init_exports(lua_State *L);
	static void frame(uv_timer_t *p);
	void update(float elapsed);
	static void count(uv_timer_t *p);
	void schedule(lua_State *L);

private:
	static int trap_current(lua_State *L);
	static int trap_task(lua_State *L);
	static void trap_sleep_done(uv_timer_t *p);
	static int trap_sleep(lua_State *L);

private:
	uv_loop_t *m_loop;
	std::thread m_thread;
	std::mutex m_mutex;
	std::condition_variable_any m_cond;
	state_t m_state;
	std::string m_rootfs;
	std::string m_initrc;
	uv_async_t m_host_async;
	uv_async_t m_quit_async;

private:
	int64_t m_last;
	sge_list_t m_task_list;
	int m_frame_count;
	int m_frame_per_second;
};

SGE_INLINE const std::string &kernel::rootfs(void) const
{
	return m_rootfs;
}

SGE_INLINE const std::string &kernel::initrc(void) const
{
	return m_initrc;
}

SGE_VM_END

#endif // SGE_VM_KERNEL_HPP
