/*
 *
 */
#ifndef SGE_VMTASK_H
#define SGE_VMTASK_H

#include <uv.h>
#include <cx/list.h>

#ifdef LUA_EXTRASPACE
#	undef LUA_EXTRASPACE
#endif

#ifdef luai_userstatethread
#	undef luai_userstatethread
#endif

#ifdef luai_userstatefree
#	undef luai_userstatefree
#endif

#ifdef luai_userstateresume
#	undef luai_userstateresume
#endif

#ifdef luai_userstateyield
#	undef luai_userstateyield
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	cx_list_node_t node;
	uv_timer_t sleepTimer;
	void *vm;
} SGEVMTask;

extern void sgeVMTaskAddedHook(lua_State *L, lua_State *L1);
extern void sgeVMTaskRemovedHook(lua_State *L, lua_State *L1);
extern void sgeVMTaskResumeHook(lua_State *L, int n);
extern void sgeVMTaskYieldHook(lua_State *L, int n);

#ifdef __cplusplus
}
#endif

#define LUA_EXTRASPACE sizeof(SGEVMTask)

#define luai_userstatethread(L, L1) sgeVMTaskAddedHook(L, L1)
#define luai_userstatefree(L, L1) sgeVMTaskRemovedHook(L, L1)
#define luai_userstateresume(L, n) sgeVMTaskResumeHook(L, n)
#define luai_userstateyield(L, n) sgeVMTaskYieldHook(L, n)

#endif /* SGE_VMTASK_H */
