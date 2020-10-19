//
//
#ifndef SGE_BOOT_ENGINE_HPP
#define SGE_BOOT_ENGINE_HPP

#include <sge/vm/kernel.hpp>
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
    void render(const vm::view &v) override;

private:
    graphics::window m_window;
    graphics::renderer m_renderer;
};

SGE_BOOT_END

#endif // SGE_BOOT_ENGINE_HPP
