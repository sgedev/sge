/*
 *
 */
#ifndef SGE_VM_H
#define SGE_VM_H

#include <uv.h>

#include <cx/list.h>

#ifdef LoadString
#undef LoadString
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	cx_list_node_t node;
	uv_timer_t sleep_timer;
	void *data;
} sge_vm_task_t;

void sge_vm_init(lua_State* L);
void sge_vm_shutdown(lua_State* L);
void sge_vm_add_task(lua_State* L, lua_State* L1);
void sge_vm_remove_task(lua_State* L, lua_State* L1);
void sge_vm_resume_task(lua_State* L, int n);
void sge_vm_yield_task(lua_State* L, int n);

#ifdef __cplusplus
}
#endif

#ifdef LUA_EXTRASPACE
#undef LUA_EXTRASPACE
#endif
#define LUA_EXTRASPACE sizeof(sge_vm_task_t)

#if defined(luai_userstateopen)
#undef luai_userstateopen
#endif
#define luai_userstateopen sge_vm_init

#if defined(luai_userstateclose)
#undef luai_userstateclose
#endif
#define luai_userstateclose sge_vm_shutdown

#if defined(luai_userstatethread)
#undef luai_userstatethread
#endif
#define luai_userstatethread sge_vm_add_task

#if defined(luai_userstatefree)
#undef luai_userstatefree
#endif
#define luai_userstatefree sge_vm_remove_task

#if defined(luai_userstateresume)
#undef luai_userstateresume
#endif
#define luai_userstateresume sge_vm_resume_task

#if defined(luai_userstateyield)
#undef luai_userstateyield
#endif
#define luai_userstateyield sge_vm_yield_task

#endif /* SGE_VM_H */

