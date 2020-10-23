//
//
#include <sge/app/client.hpp>

SGE_APP_BEGIN

client::client(uv_loop_t *loop):
    engine(loop)
{
}

client::~client(void)
{
}

bool client::start(const std::string &rootfs, const std::string &initrc)
{
    if (!m_window.init("sge", 800, 600, SDL_WINDOW_RESIZABLE))
        return false;

    if (!m_renderer.init(&m_window)) {
        m_window.shutdown();
        return false;
    }

    if (!engine::start(rootfs, initrc)) {
        m_renderer.shutdown();
        m_window.shutdown();
        return false;
    }

    return true;
}

void client::stop(void)
{
    engine::stop();
    m_renderer.shutdown();
    m_window.shutdown();
}

void client::handle_event(const SDL_Event &evt)
{
    if (evt.type != SDL_WINDOWEVENT)
        vm::kernel::handle_event(evt);
    else
        m_window.handle_event(evt.window);
}

void client::render(const vm::view &v)
{
    m_renderer.begin();

    // TODO

    m_renderer.end();
}

void client::trap_foo(void)
{
}

SGE_APP_END
