/*
 *
 */
#ifndef SGE_GAME_H
#define SGE_GAME_H

#include <uv.h>

#ifdef LoadString
#	undef LoadString
#endif

#include <cx/list.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	cx_list_node_t node;
} SGEGameTask;

#define SGE_GAME_TASK_FROM_LUA(L) \
	((SGEGameTask *)lua_getextraspace(L))

#define SGE_GAME_TASK_TO_LUA(task) \
	((lua_State *)CX_PMOVB(task, LUA_EXTRASPACE))

void SGEGameInitHook(lua_State *L);
void SGEGameShutdownHook(lua_State *L);
void SGEGameAddTaskHook(lua_State *L, lua_State *L1);
void SGEGameRemoveTaskHook(lua_State *L, lua_State *L1);
void SGEGameResumeTaskHook(lua_State *L, int n);
void SGEGameYieldTaskHook(lua_State *L, int n);

#ifdef __cplusplus
}
#endif

#ifdef LUA_EXTRASPACE
#	undef LUA_EXTRASPACE
#endif
#define LUA_EXTRASPACE sizeof(SGEGameTask)

#ifdef luai_userstateopen
#	undef luai_userstateopen
#endif
#define luai_userstateopen(L) SGEGameInitHook(L)

#ifdef luai_userstateclose
#	undef luai_userstateclose
#endif
#define luai_userstateclose(L) SGEGameShutdownHook(L)

#ifdef luai_userstatethread
#	undef luai_userstatethread
#endif
#define luai_userstatethread(L, L1) SGEGameAddTaskHook(L, L1)

#ifdef luai_userstatefree
#	undef luai_userstatefree
#endif
#define luai_userstatefree(L, L1) SGEGameRemoveTaskHook(L, L1)

#ifdef luai_userstateresume
#	undef luai_userstateresume
#endif
#define luai_userstateresume(L, n) SGEGameResumeTaskHook(L, n)

#ifdef luai_userstateyield
#	undef luai_userstateyield
#endif
#define luai_userstateyield(L, n) SGEGameYieldTaskHook(L, n)

#endif /* SGE_GAME_H */