//
//
#ifndef SGE_VM_CORE_HPP
#define SGE_VM_CORE_HPP

#include <string>
#include <thread>
#include <future>

#include <sge/spin_lock.hpp>
#include <sge/scene/world.hpp>
#include <sge/graphics/window.hpp>
#include <sge/graphics/renderer.hpp>
#include <sge/graphics/view.hpp>
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
    bool running(void) const;
    void post_event(const SDL_Event &evt);
    const std::thread &thread(void) const;
    root_fs &rootfs(void);
    scene::world &world(void);
    graphics::window &window(void);
    graphics::renderer &renderer(void);

protected:
    void set_init_result(bool v);
    void kick(void);
    virtual void run(uv_loop_t *loop);
    virtual void handle_event(const SDL_Event &evt);
    virtual void frame(float elapsed);

private:
    void thread_main(void);
    static void frame_cb(uv_timer_t *p);
    static void frame_count_cb(uv_timer_t *p);
    static void quit_cb(uv_async_t *p);
    static void kick_cb(uv_async_t *p);
    static void view_update_cb(uv_async_t *p);

private:
    static const int EVENT_QUEUE_ORDER = 6;
    static const int EVENT_QUEUE_SIZE = 1 << EVENT_QUEUE_ORDER;
    static const int EVENT_QUEUE_MASK = EVENT_QUEUE_SIZE - 1;

private:
    uv_loop_t *m_loop;
    bool m_running;
    std::thread m_thread;
    std::string m_rootfs_path;
    std::string m_initrc_path;
    std::promise<bool> m_init_promise;
    uv_async_t m_quit_async;
    uv_async_t m_kick_async;
    SDL_Event m_event_queue[EVENT_QUEUE_SIZE];
    int64_t m_event_first;
    int64_t m_event_last;
    int64_t m_last;
    int m_frame_count;
    int m_frame_per_second;
    root_fs m_root_fs;
    scene::world m_world;
    graphics::window m_window;
    graphics::renderer m_renderer;
    graphics::view m_view_pool[2];
    int m_view_rendering;
    spin_lock m_view_lock;
    uv_async_t m_view_update_async;
};

SGE_INLINE uv_loop_t *core::loop(void)
{
    return m_loop;
}

SGE_INLINE bool core::running(void) const
{
    return m_running;
}

SGE_INLINE const std::thread &core::thread(void) const
{
    SGE_ASSERT(running());

    return m_thread;
}

SGE_INLINE root_fs &core::rootfs(void)
{
    SGE_ASSERT(running());
    SGE_ASSERT(std::this_thread::get_id() == thread().get_id());

    return m_root_fs;
}

SGE_INLINE scene::world &core::world(void)
{
    SGE_ASSERT(running());
    SGE_ASSERT(std::this_thread::get_id() == thread().get_id());

    return m_world;
}

SGE_INLINE graphics::window &core::window(void)
{
    SGE_ASSERT(running());
    SGE_ASSERT(std::this_thread::get_id() != thread().get_id());

    return m_window;
}

SGE_INLINE graphics::renderer &core::renderer(void)
{
    SGE_ASSERT(running());
    SGE_ASSERT(std::this_thread::get_id() != m_thread.get_id());

    return m_renderer;
}

SGE_INLINE void core::kick(void)
{
    SGE_ASSERT(running());
    SGE_ASSERT(std::this_thread::get_id() != m_thread.get_id());

    uv_async_send(&m_kick_async);
}

SGE_VM_END

#endif // SGE_VM_CORE_HPP
