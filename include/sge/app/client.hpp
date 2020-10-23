//
//
#ifndef SGE_APP_CLIENT_HPP
#define SGE_APP_CLIENT_HPP

#include <sge/graphics/window.hpp>
#include <sge/graphics/renderer.hpp>
#include <sge/app/engine.hpp>

SGE_APP_BEGIN

class client: public engine {
public:
    client(uv_loop_t *loop);
    ~client(void) override;

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
    graphics::window m_window;
    graphics::renderer m_renderer;
    uv_async_t m_render_view_async;
    int m_view_rendering;
    spin_lock m_view_mutex;
};

SGE_APP_END

#endif // SGE_APP_CLIENT_HPP
