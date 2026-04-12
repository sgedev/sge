//
//
#ifndef SGE_SYSTEM_BASE_HPP
#define SGE_SYSTEM_BASE_HPP

#include <sge/vm/context.hpp>
#include <sge/system/common.hpp>

SGE_SYSTEM_BEGIN

class Base: public vm::Context {
public:
    Base() = default;
    virtual ~Base() = default;

public:
    void initSyscalls(lua_State* L) override;
    int trap(lua_State* T) override;
    virtual void run() = 0;

protected:
    virtual void frame(Clock::duration elapsed);

private:
    void handleTraps();

private:
};

SGE_SYSTEM_END

#endif // SGE_SYSTEM_BASE_HPP
