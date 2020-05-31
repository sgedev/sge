/*
 *
 */
#ifndef SGE_LUA_H
#define SGE_LUA_H

#include <uv.h>
#include <cx/list.h>

#ifdef LUA_EXTRASPACE
#	undef LUA_EXTRASPACE
#endif

#ifdef luai_userstateopen
#	undef luai_userstateopen
#endif

#ifdef luai_userstateclose
#	undef luai_userstateclose
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
} SGELuaTask;

extern void sgeLuaInitHook(lua_State *L);
extern void sgeLuaShutdownHook(lua_State *L);
extern void sgeLuaTaskNewHook(lua_State *L, lua_State *L1);
extern void sgeLuaTaskFreeHook(lua_State *L, lua_State *L1);
extern void sgeLuaTaskResumeHook(lua_State *L, int n);
extern void sgeLuaTaskYieldHook(lua_State *L, int n);

#ifdef __cplusplus
}
#endif

#define LUA_EXTRASPACE sizeof(SGELuaTask)

#define luai_userstateopen(L) sgeLuaInitHook(L)
#define luai_userstateclose(L) sgeLuaShutdownHook(L)
#define luai_userstatethread(L, L1) sgeLuaTaskNewHook(L, L1)
#define luai_userstatefree(L, L1) sgeLuaTaskFreeHook(L, L1)
#define luai_userstateresume(L, n) sgeLuaTaskResumeHook(L, n)
#define luai_userstateyield(L, n) sgeLuaTaskYieldHook(L, n)

#endif /* SGE_VM_TASK_H */
