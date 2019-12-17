/*
 *
 */
#ifndef SGE_LUA_H
#define SGE_LUA_H

#include <cx/list.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	CXListNode node;
	void *game;
	void *task;
} SGEGameContext;

void SGEGameLuaInitHook(lua_State *L);
void SGEGameLuaShutdownHook(lua_State *L);
void SGEGameLuaAddTaskHook(lua_State *L, lua_State *L1);
void SGEGameLuaRemoveTaskHook(lua_State *L, lua_State *L1);
void SGEGameLuaResumeTaskHook(lua_State *L, int n);
void SGEGameLuaYieldTaskHook(lua_State *L, int n);

#ifdef __cplusplus
}
#endif

#ifdef LUA_EXTRASPACE
#undef LUA_EXTRASPACE
#endif
#define LUA_EXTRASPACE sizeof(SGEGameContext)

#ifdef luai_userstateopen
#undef luai_userstateopen
#endif
#define luai_userstateopen(L) SGEGameLuaInitHook(L)

#ifdef luai_userstateclose
#undef luai_userstateclose
#endif
#define luai_userstateclose(L) SGEGameLuaShutdownHook(L)

#ifdef luai_userstatethread
#undef luai_userstatethread
#endif
#define luai_userstatethread(L, L1) SGEGameLuaAddTaskHook(L, L1)

#ifdef luai_userstatefree
#undef luai_userstatefree
#endif
#define luai_userstatefree(L, L1) SGEGameLuaRemoveTaskHook(L, L1)

#ifdef luai_userstateresume
#undef luai_userstateresume
#endif
#define luai_userstateresume(L, n) SGEGameLuaResumeTaskHook(L, n)

#ifdef luai_userstateyield
#undef luai_userstateyield
#endif
#define luai_userstateyield(L, n) SGEGameLuaYieldTaskHook(L, n)

#endif /* SGE_LUA_H */
