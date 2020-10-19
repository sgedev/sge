//
//
#include <sge/boot/engine.hpp>

SGE_BOOT_BEGIN

engine::engine(uv_loop_t *loop):
    vm::kernel(loop)
{
}

engine::~engine(void)
{
}

bool engine::start(const std::string &rootfs, const std::string &initrc)
{
    if (!m_window.init("sge", 800, 600, SDL_WINDOW_RESIZABLE))
        return false;

    if (!m_renderer.init(&m_window)) {
        m_window.shutdown();
        return false;
    }

    if (!vm::kernel::start(rootfs, initrc)) {
        m_renderer.shutdown();
        m_window.shutdown();
        return false;
    }

    return true;
}

void engine::stop(void)
{
    vm::kernel::stop();
    m_renderer.shutdown();
    m_window.shutdown();
}

void engine::render(const vm::view &v)
{
    m_renderer.begin();

    // TODO

    m_renderer.end();
}

SGE_BOOT_END
