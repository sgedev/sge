//
//
#ifndef SGE_BOOT_ENGINE_HPP
#define SGE_BOOT_ENGINE_HPP

#include <sge/vm/kernel.hpp>
#include <sge/scene/world.hpp>
#include <sge/graphics/window.hpp>
#include <sge/graphics/renderer.hpp>
#include <sge/boot/common.hpp>

SGE_BOOT_BEGIN

class engine: public vm::kernel {
public:
    engine(uv_loop_t *loop);
    ~engine(void) override;

public:
    bool start(const std::string &rootfs, const std::string &initrc) override;
    void stop(void) override;

protected:
    void handle_event(const SDL_Event &evt) override;
    void trap_foo(void) override;

private:
    static const int VIEW_COUNT = 2;

    static void render_view(uv_async_t *p);

private:
    scene::world m_world;
    graphics::window m_window;
    graphics::renderer m_renderer;
    uv_async_t m_render_view_async;
    int m_view_rendering;
    spin_lock m_view_mutex;
};

SGE_BOOT_END

#endif // SGE_BOOT_ENGINE_HPP
