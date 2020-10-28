//
//
#include <sge/vm/core.hpp>

SGE_VM_BEGIN

core::core(uv_loop_t *loop):
    m_loop(loop),
    m_running(false)
{
    SGE_ASSERT(m_loop != nullptr);
}

core::~core(void)
{
    if (running())
        stop();
}

bool core::start(const std::string &rootfs_path, const std::string &initrc_path)
{
    SGE_ASSERT(!running());
    SGE_ASSERT(m_loop != nullptr);

    if (!m_window.init("sge", 800, 600, SDL_WINDOW_RESIZABLE))
        return false;

    if (!m_renderer.init(&m_window)) {
        m_window.shutdown();
        return false;
    }

    m_rootfs_path = rootfs_path;
    m_initrc_path = initrc_path;
    m_event_first = 0;
    m_event_last = 0;

    uv_async_init(m_loop, &m_view_update_async, &core::view_update_cb);
    m_view_update_async.data = this;

    std::future<bool> init_result = m_init_promise.get_future();
    m_thread = std::thread(&core::thread_main, this);
    m_running = init_result.get();

    return m_running;
}

void core::stop(void)
{
    SGE_ASSERT(running());

    uv_async_send(&m_quit_async);
    m_thread.join();

    uv_sync_close(&m_view_update_async);

    m_renderer.shutdown();
    m_window.shutdown();

    m_running = false;
}

void core::post_event(const SDL_Event &evt)
{
    SGE_ASSERT(running());

    if (evt.type != SDL_WINDOWEVENT) {
        m_event_queue[m_event_last & EVENT_QUEUE_MASK] = evt;
        m_event_last += 1;
        return;
    }

    m_window.handle_event(evt.window);
}

void core::set_init_result(bool v)
{
    m_init_promise.set_value(v);
}

void core::run(uv_loop_t *loop)
{
    uv_async_init(loop, &m_quit_async, &core::quit_cb);
    m_quit_async.data = this;

    uv_async_init(loop, &m_kick_async, &core::kick_cb);
    m_kick_async.data = this;

    uv_timer_t frame_timer;
    uv_timer_t frame_count_timer;

    uv_timer_init(loop, &frame_timer);
    uv_timer_init(loop, &frame_count_timer);

    uv_timer_start(&frame_timer, &core::frame_cb, 0, 16);
    uv_timer_start(&frame_count_timer, &core::frame_count_cb, 1000, 1000);

    frame_timer.data = this;
    frame_count_timer.data = this;

    m_last = uv_now(loop);
    m_frame_count = 0;
    m_frame_per_second = 0;

    m_init_promise.set_value(true);

    uv_run(loop, UV_RUN_DEFAULT);

    uv_timer_stop(&frame_count_timer);
    uv_timer_stop(&frame_timer);

    uv_sync_close(&frame_count_timer);
    uv_sync_close(&frame_timer);

    uv_sync_close(&m_kick_async);
    uv_sync_close(&m_quit_async);
}

void core::handle_event(const SDL_Event &evt)
{
    m_world.handle_event(evt);
}

void core::frame(float elapsed)
{
    while (m_event_first < m_event_last) {
        handle_event(m_event_queue[m_event_first & EVENT_QUEUE_MASK]);
        m_event_first += 1;
    }

    m_world.update(elapsed);

    graphics::view &v = m_view_pool[!m_view_rendering];
    // TOOD build view to 'v'.

    std::unique_lock locker(m_view_lock);
    uv_async_send(&m_view_update_async);
    m_view_rendering = !m_view_rendering;
}

void core::render(const graphics::view &v)
{
    m_renderer.render(v);
}

void core::thread_main(void)
{
    if (!m_root_fs.init(m_rootfs_path)) {
        m_init_promise.set_value(false);
        return;
    }

    if (!m_world.init()) {
        m_init_promise.set_value(false);
        return;
    }

    uv_loop_t loop;
    uv_loop_init(&loop);

    run(&loop);

    uv_loop_close(&loop);
}

void core::frame_cb(uv_timer_t *p)
{
    core *c = reinterpret_cast<core *>(p->data);

    int64_t curr = uv_now(p->loop);
    int64_t pass = curr - c->m_last;
    if (pass <= 0)
        return;

    c->frame(float(pass) / 1000.0f);
    c->m_last = curr;
    c->m_frame_count += 1;

    uv_update_time(p->loop);
}

void core::frame_count_cb(uv_timer_t *p)
{
    core *c = reinterpret_cast<core *>(p->data);
    c->m_frame_per_second = c->m_frame_count;
    c->m_frame_count = 0;
}

void core::quit_cb(uv_async_t *p)
{
    uv_stop(p->loop);
}

void core::kick_cb(uv_async_t *p)
{
}

void core::view_update_cb(uv_async_t *p)
{
    core *c = reinterpret_cast<core *>(p->data);
    std::unique_lock locker(c->m_view_lock);
    c->render(c->m_view_pool[c->m_view_rendering]);
}

SGE_VM_END
