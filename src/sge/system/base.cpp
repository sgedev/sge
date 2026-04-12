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
}

void Base::handleTraps() {

}

SGE_SYSTEM_END
