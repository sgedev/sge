/*
 *
 */
#ifndef SGE_VM_LUA_H
#define SGE_VM_LUA_H

#ifdef LUA_EXTRASPACE
#   undef LUA_EXTRASPACE
#endif

#ifndef luai_userstateopen
#   undef luai_userstateopen
#endif

#ifndef luai_userstateclose
#   undef luai_userstateclose
#endif

#ifndef luai_userstatethread
#   undef luai_userstatethread
#endif

#ifndef luai_userstatefree
#   undef luai_userstatefree
#endif

#ifndef luai_userstateresume
#   undef luai_userstateresume
#endif

#ifndef luai_userstateyield
#   undef luai_userstateyield
#endif

#include <uv.h>
#include <cx/list.h>

#ifdef _WIN32
#   undef LoadString
#endif

typedef struct {
    cx_list_node_t node;
    uv_timer_t sleep_timer;
} sge_vm_task_t;

void sge_vm_start(lua_State *L);
void sge_vm_stop(lua_State *L);
void sge_vm_task_add(lua_State *L, lua_State *L1);
void sge_vm_task_remove(lua_State *L, lua_State *L1);
void sge_vm_task_resume(lua_State *L, int n);
void sge_vm_task_yield(lua_State *L, int n);

#define LUA_EXTRASPACE sizeof(sge_vm_task_t)

#define luai_userstateopen(L) sge_vm_start(L)
#define luai_userstateclose(L) sge_vm_stop(L)
#define luai_userstatethread(L, L1) sge_vm_task_add(L, L1)
#define luai_userstatefree(L, L1) sge_vm_task_remove(L, L1)
#define luai_userstateresume(L, n) sge_vm_task_resume(L, n)
#define luai_userstateyield(L, n) sge_vm_task_yield(L, n)

#endif /* SGE_VM_LUA_H */
