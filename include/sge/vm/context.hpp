//
//
#ifndef SGE_VM_CONTEXT_HPP
#define SGE_VM_CONTEXT_HPP

#include <chrono>

#include <sge/vm/common.hpp>

SGE_VM_BEGIN

class Context {
public:
    using Clock = std::chrono::steady_clock;

public:
    Context() = default;
    Context(const Context&) = delete;
    Context(Context&&) = delete;
    virtual ~Context() = default;

public:
    Context& operator=(const Context&) = delete;
    Context& operator=(Context&&) = delete;

    virtual void exit() = 0;
    virtual void initSyscalls(lua_State* L) = 0;
    virtual int trap(lua_State* T) = 0;
};

SGE_VM_END

#endif // SGE_VM_CONTEXT_HPP
