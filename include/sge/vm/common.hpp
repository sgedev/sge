//
//
#ifndef SGE_VM_COMMON_HPP
#define SGE_VM_COMMON_HPP

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <sge/common.hpp>

#define SGE_VM_BEGIN SGE_BEGIN namespace vm {
#define SGE_VM_END } SGE_END

SGE_VM_BEGIN

using Task = sge_vm_Task;

SGE_INLINE lua_State* taskToState(Task* task) {
    return (lua_State*)SGE_PMOVB(task, LUA_EXTRASPACE);
}

SGE_INLINE Task* taskFromState(lua_State* L) {
    return (sge_vm_Task*)lua_getextraspace(L);
}

SGE_VM_END

#endif // SGE_VM_COMMON_HPP
