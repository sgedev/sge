//
//
#ifndef SGE_VM_CORE_HPP
#define SGE_VM_CORE_HPP

#include <array>
#include <string>
#include <functional>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

#include <sge/scene/world.hpp>

#include <sge/vm/common.hpp>

SGE_VM_BEGIN

class core {
public:
	static const int EVENT_QUEUE_SIZE = 64;

public:
	core(uv_loop_t *loop);
	virtual ~core(void);

public:
	bool start(const std::string &rootfs_path, const std::string &initrc_path);
	void stop(void);
	void post_event(const SDL_Event &evt);
	const std::thread &thread(void) const;
	const std::string &rootfs_path(void) const;
	const std::string &initrc_path(void) const;

protected:
	void set_init_result(bool v);
	void host_run(std::function<void (void)> f);
	virtual void run(uv_loop_t *loop);
	virtual void handle_event(const SDL_Event &evt);

private:
	static void host(uv_async_t *p);
	static void quit(uv_async_t *p);
	bool init_rootfs(void);
	void update(float elapsed);
	static void frame(uv_timer_t *p);
	static void count(uv_timer_t *p);

private:
	uv_loop_t *m_host_loop;
	std::thread m_thread;
	std::promise<bool> m_init_promise;
	std::string m_rootfs_path;
	std::string m_initrc_path;
	uv_async_t m_host_async;
	uv_async_t m_quit_async;

private:
	SDL_Event m_event_queue[EVENT_QUEUE_SIZE];
	int64_t m_event_first;
	int64_t m_event_last;

private:
	int64_t m_last;
	uv_timer_t m_frame_timer;
	uv_timer_t m_count_timer;
	int m_frame_count;
	int m_frame_per_second;
	scene::world m_world;
};

SGE_INLINE const std::thread &core::thread(void) const
{
	return m_thread;
}

SGE_INLINE const std::string &core::rootfs_path(void) const
{
	return m_rootfs_path;
}

SGE_INLINE const std::string &core::initrc_path(void) const
{
	return m_initrc_path;
}

SGE_VM_END

#endif // SGE_VM_CORE_HPP
