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
} SGEGameInfo;

void SGEGameLuaInit(lua_State *L);
void SGEGameLuaShutdown(lua_State *L);
void SGEGameLuaAddTask(lua_State *L, lua_State *L1);
void SGEGameLuaRemoveTask(lua_State *L, lua_State *L1);
void SGEGameLuaResumeTask(lua_State *L, int n);
void SGEGameLuaYieldTask(lua_State *L, int n);

#ifdef __cplusplus
}
#endif

#ifdef LUA_EXTRASPACE
#undef LUA_EXTRASPACE
#endif
#define LUA_EXTRASPACE sizeof(SGEGameInfo)

#ifdef luai_userstateopen
#undef luai_userstateopen
#endif
#define luai_userstateopen(L) SGEGameLuaInit(L)

#ifdef luai_userstateclose
#undef luai_userstateclose
#endif
#define luai_userstateclose(L) SGEGameLuaShutdown(L)

#ifdef luai_userstatethread
#undef luai_userstatethread
#endif
#define luai_userstatethread(L, L1) SGEGameLuaAddTask(L, L1)

#ifdef luai_userstatefree
#undef luai_userstatefree
#endif
#define luai_userstatefree(L, L1) SGEGameLuaRemoveTask(L, L1)

#ifdef luai_userstateresume
#undef luai_userstateresume
#endif
#define luai_userstateresume(L, n) SGEGameLuaResumeTask(L, n)

#ifdef luai_userstateyield
#undef luai_userstateyield
#endif
#define luai_userstateyield(L, n) SGEGameLuaYieldTask(L, n)

#endif /* SGE_LUA_H */
