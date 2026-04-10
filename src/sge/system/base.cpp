//
//
#include <sge/system/base.hpp>

SGE_SYSTEM_BEGIN

void Base::initSyscalls(lua_State* L) {
}

int Base::trap(lua_State* T) {
    return 0;
}

void Base::frame(Clock::duration elapsed) {
    handleTraps();

    auto delta = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000000.0f;
    //physics_system_.Update(delta, 1);
}

void Base::handleTraps() {

}

SGE_SYSTEM_END
