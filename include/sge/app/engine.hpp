//
//
#ifndef SGE_APP_ENGINE_HPP
#define SGE_APP_ENGINE_HPP

#include <sge/vm/kernel.hpp>
#include <sge/scene/world.hpp>
#include <sge/app/common.hpp>

SGE_APP_BEGIN

class engine: public vm::kernel {
public:
    engine(uv_loop_t *loop);
    ~engine(void) override;

public:
    bool start(const std::string &rootfs, const std::string &initrc) override;
    void stop(void) override;

private:
    scene::world m_world;
};

SGE_APP_END

#endif // SGE_APP_ENGINE_HPP
