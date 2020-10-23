//
//
#include <sge/vm/core.hpp>

SGE_VM_BEGIN

core::core(uv_loop_t *loop):
    m_loop(loop)
{
    SGE_ASSERT(m_loop != nullptr);
}

core::~core(void)
{
	stop();
}

bool core::start(const std::string &rootfs_path, const std::string &initrc_path)
{
    SGE_ASSERT(m_loop != nullptr);

	m_rootfs_path = rootfs_path;
	m_initrc_path = initrc_path;
	m_event_first = 0;
	m_event_last = 0;

	std::future<bool> init_result = m_init_promise.get_future();
    m_thread = std::thread(&core::thread_main, this);

    return init_result.get();
}

void core::stop(void)
{
	if (m_thread.joinable()) {
		uv_async_send(&m_quit_async);
		m_thread.join();
	}
}

void core::post_event(const SDL_Event &evt)
{
    m_event_queue[m_event_last & EVENT_QUEUE_MASK] = evt;
	m_event_last += 1;
}

void core::set_init_result(bool v)
{
    m_init_promise.set_value(v);
}

void core::run(uv_loop_t *loop)
{
    uv_async_init(loop, &m_quit_async, &core::quit);
    m_quit_async.data = this;

    uv_timer_t frame_timer;
    uv_timer_t count_timer;

    uv_timer_init(loop, &frame_timer);
    uv_timer_init(loop, &count_timer);

    uv_timer_start(&frame_timer, &core::frame, 0, 16);
    uv_timer_start(&count_timer, &core::count, 1000, 1000);

    frame_timer.data = this;
    count_timer.data = this;

    m_last = uv_now(loop);
    m_frame_count = 0;
    m_frame_per_second = 0;

    m_init_promise.set_value(true);

    uv_run(loop, UV_RUN_DEFAULT);

    uv_timer_stop(&count_timer);
    uv_timer_stop(&frame_timer);

    uv_sync_close(&count_timer);
    uv_sync_close(&frame_timer);
    uv_sync_close(&m_quit_async);
}

void core::handle_event(const SDL_Event &evt)
{
}

void core::thread_main(void)
{
    if (!m_root_fs.init(m_rootfs_path)) {
        m_init_promise.set_value(false);
        return;
    }

    uv_loop_t loop;
    uv_loop_init(&loop);

    run(&loop);

    uv_loop_close(&loop);
}

void core::update(float elapsed)
{
	while (m_event_first < m_event_last) {
        handle_event(m_event_queue[m_event_first & EVENT_QUEUE_MASK]);
		m_event_first += 1;
	}
}

void core::frame(uv_timer_t *p)
{
	core *c = reinterpret_cast<core *>(p->data);

	int64_t curr = uv_now(p->loop);
	int64_t pass = curr - c->m_last;
	if (pass <= 0)
		return;

    c->update(float(pass) / 1000.0f);
	c->m_last = curr;
	c->m_frame_count += 1;

	uv_update_time(p->loop);
}

void core::count(uv_timer_t *p)
{
	core *c = reinterpret_cast<core *>(p->data);
	c->m_frame_per_second = c->m_frame_count;
	c->m_frame_count = 0;
    SGE_LOGD("fps %d", c->m_frame_per_second);
}

void core::quit(uv_async_t *p)
{
	uv_stop(p->loop);
}

SGE_VM_END
