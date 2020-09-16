/*
 *
 */
#ifndef SGE_VM_TASK_H
#define SGE_VM_TASK_H

#include <uv.h>

#include <cx/list.h>

#ifdef LUA_EXTRASPACE
#	undef LUA_EXTRASPACE
#endif

#ifdef luai_userstatethread
#undef	luai_userstatethread
#endif

#ifdef luai_userstatefree
#undef	luai_userstatefree
#endif

#ifdef luai_userstateresume
#undef	luai_userstateresume
#endif

#ifdef luai_userstateyield
#undef	luai_userstateyield
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sge_vm_task {
	cx_list_node_t node;
    uv_loop_t *loop;
	uv_timer_t sleep_timer;
} sge_vm_task_t;

void sge_vm_task_added(lua_State *L, lua_State *L1);
void sge_vm_task_removed(lua_State *L, lua_State *L1);
void sge_vm_task_resume(lua_State *L, int n);
void sge_vm_task_yield(lua_State *L, int n);

#ifdef __cplusplus
}
#endif

#define LUA_EXTRASPACE sizeof(sge_vm_task_t)

#define luai_userstatethread(L, L1) sge_vm_task_added(L, L1)
#define luai_userstatefree(L, L1) sge_vm_task_removed(L, L1)
#define luai_userstateresume(L, n) sge_vm_task_resume(L, n)
#define luai_userstateyield(L, n) sge_vm_task_yield(L, n)

#endif /* SGE_VM_TASK_H */
