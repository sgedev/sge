//
//
#include <sge/vm/core.hpp>

SGE_VM_BEGIN

core::core(uv_loop_t *loop):
	m_host_loop(loop)
{
	SGE_ASSERT(m_host_loop != nullptr);

	uv_async_init(loop, &m_host_async, &core::host);
	m_host_async.data = this;
}

core::~core(void)
{
	stop();

	uv_close(reinterpret_cast<uv_handle_t *>(&m_host_async), nullptr);
}

bool core::start(const std::string &rootfs_path, const std::string &initrc_path)
{
	SGE_ASSERT(m_host_loop != nullptr);

	uv_async_init(m_host_loop, &m_host_async, &core::host);

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
	m_event_queue[m_event_last] = evt;
	m_event_last += 1;
}

void core::run(uv_loop_t *loop)
{
	uv_async_init(loop, &m_quit_async, &core::quit);
	m_quit_async.data = this;

	uv_timer_init(loop, &m_frame_timer);
	uv_timer_start(&m_frame_timer, &core::frame, 16, 16);
	m_frame_timer.data = this;

	uv_timer_init(loop, &m_count_timer);
	uv_timer_start(&m_count_timer, &core::count, 1000, 1000);
	m_count_timer.data = this;

	m_last = uv_now(loop);
	m_frame_count = 0;
	m_frame_per_second = 0;

	m_init_promise.set_value(true);

	uv_run(loop, UV_RUN_DEFAULT);

	uv_timer_stop(&m_count_timer);
	uv_timer_stop(&m_frame_timer);

	uv_close(reinterpret_cast<uv_handle_t *>(&m_count_timer), nullptr);
	uv_close(reinterpret_cast<uv_handle_t *>(&m_frame_timer), nullptr);
	uv_close(reinterpret_cast<uv_handle_t *>(&m_quit_async), nullptr);
}

void core::handle_event(const SDL_Event &evt)
{
	m_world.handle_event(evt);
}

void core::set_init_result(bool v)
{
	m_init_promise.set_value(v);
}

void core::host_run(std::function<void(void)> f)
{
}

void core::update(float elapsed)
{
	while (m_event_first < m_event_last) {
		handle_event(m_event_queue[m_event_first]);
		m_event_first += 1;
	}

	m_world.update(elapsed);
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

    while (uv_loop_close(&loop) == UV_EBUSY)
        uv_run(&loop, UV_RUN_ONCE);
}

void core::host(uv_async_t *p)
{
}

void core::quit(uv_async_t *p)
{
	uv_stop(p->loop);
}

SGE_VM_END
