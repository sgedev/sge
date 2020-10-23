//
//
#include <sge/app/engine.hpp>

SGE_APP_BEGIN

engine::engine(uv_loop_t *loop):
    vm::kernel(loop)
{
}

engine::~engine(void)
{
}

bool engine::start(const std::string &rootfs, const std::string &initrc)
{
    if (!vm::kernel::start(rootfs, initrc))
        return false;

    return true;
}

void engine::stop(void)
{
    vm::kernel::stop();
}

SGE_APP_END
