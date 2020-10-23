//
//
#ifndef SGE_VM_CORE_HPP
#define SGE_VM_CORE_HPP

#include <string>
#include <thread>
#include <future>

#include <sge/spin_lock.hpp>
#include <sge/vm/common.hpp>
#include <sge/vm/root_fs.hpp>

SGE_VM_BEGIN

class core {
public:
	core(uv_loop_t *loop);
	virtual ~core(void);

public:
    virtual bool start(const std::string &rootfs_path, const std::string &initrc_path);
    virtual void stop(void);
    uv_loop_t *loop(void);
	void post_event(const SDL_Event &evt);
	const std::thread &thread(void) const;
    root_fs &rootfs(void);

protected:
    void set_init_result(bool v);
    virtual void run(uv_loop_t *loop);
    virtual void handle_event(const SDL_Event &evt);
    virtual void update(float elapsed);

private:
    void thread_main(void);
    static void frame(uv_timer_t *p);
    static void count(uv_timer_t *p);
    static void quit(uv_async_t *p);

private:
    static const int EVENT_QUEUE_ORDER = 6;
    static const int EVENT_QUEUE_SIZE = 1 << EVENT_QUEUE_ORDER;
    static const int EVENT_QUEUE_MASK = EVENT_QUEUE_SIZE - 1;

private:
    uv_loop_t *m_loop;
	std::thread m_thread;
    std::string m_rootfs_path;
    std::string m_initrc_path;
	std::promise<bool> m_init_promise;
    uv_async_t m_quit_async;
    SDL_Event m_event_queue[EVENT_QUEUE_SIZE];
	int64_t m_event_first;
    int64_t m_event_last;
	int64_t m_last;
	int m_frame_count;
	int m_frame_per_second;
    root_fs m_root_fs;
};

SGE_INLINE uv_loop_t *core::loop(void)
{
    return m_loop;
}

SGE_INLINE const std::thread &core::thread(void) const
{
	return m_thread;
}

SGE_INLINE root_fs &core::rootfs(void)
{
    return m_root_fs;
}

SGE_VM_END

#endif // SGE_VM_CORE_HPP
