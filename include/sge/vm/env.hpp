//
//
#ifndef SGE_VM_ENV_HPP
#define SGE_VM_ENV_HPP

#include <sge/vm/common.hpp>

SGE_VM_BEGIN

class Env {
public:
	Env(lua_State* T);
	Env(const Env&) = delete;
	Env(Env&&) = delete;
	~Env() = default;

public:
	Env& operator=(const Env&) = delete;
	Env& operator=(Env&&) = delete;

private:
	lua_State* T_;
};

SGE_VM_END

#endif // SGE_VM_ENV_HPP
