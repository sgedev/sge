/*
 *
 */
#ifndef SGE_VM_HOOKS_H
#define SGE_VM_HOOKS_H

#include <sge/common.h>
#include <sge/list.h>

#undef LUA_EXTRASPACE

#undef luai_userstateopen
#undef luai_userstateclose
#undef luai_userstatethread
#undef luai_userstatefree
#undef luai_userstateresume
#undef luai_userstateyield

SGE_C_BEGIN

typedef struct {
    sge_ListNode node;
    sge_List wait_list;
    void* data;
} sge_vm_Task;

void sge_vm_initHook(lua_State* L);
void sge_vm_shutdownHook(lua_State* L);
void sge_vm_taskAddedHook(lua_State* L, lua_State* T);
void sge_vm_taskRemovedHook(lua_State* L, lua_State* T);
void sge_vm_taskResumeHook(lua_State* T, int n);
void sge_vm_taskYieldHook(lua_State* T, int n);

SGE_C_END

#define LUA_EXTRASPACE sizeof(sge_vm_Task)

#define luai_userstateopen(L) sge_vm_initHook(L)
#define luai_userstateclose(L) sge_vm_shutdownHook(L)
#define luai_userstatethread(L, T) sge_vm_taskAddedHook(L, T)
#define luai_userstatefree(L, T) sge_vm_taskRemovedHook(L, T)
#define luai_userstateresume(T, n) sge_vm_taskResumeHook(T, n)
#define luai_userstateyield(T, n) sge_vm_taskYieldHook(T, n)

#endif /* SGE_VM_HOOKS_H */
