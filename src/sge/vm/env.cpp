//
//
#ifndef SGE_VM_ENV_HPP
#define SGE_VM_ENV_HPP

#include <sge/vm/common.hpp>

SGE_VM_BEGIN

Env::Env(lua_State* T)
	: T_(T) {
}

SGE_VM_END
