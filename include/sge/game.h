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
	cx_list_node_t node;
	void *game;
	void *task;
} SGEGameContext;

void SGEGameInitLua(lua_State *L);
void SGEGameShutdownLua(lua_State *L);
void SGEGameAddLuaTask(lua_State *L, lua_State *L1);
void SGEGameRemoveLuaTask(lua_State *L, lua_State *L1);
void SGEGameResumeLuaTask(lua_State *L, int n);
void SGEGameYieldLuaTask(lua_State *L, int n);

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
#define luai_userstateopen(L) SGEGameInitLua(L)

#ifdef luai_userstateclose
#undef luai_userstateclose
#endif
#define luai_userstateclose(L) SGEGameShutdownLua(L)

#ifdef luai_userstatethread
#undef luai_userstatethread
#endif
#define luai_userstatethread(L, L1) SGEGameAddLuaTask(L, L1)

#ifdef luai_userstatefree
#undef luai_userstatefree
#endif
#define luai_userstatefree(L, L1) SGEGameRemoveLuaTask(L, L1)

#ifdef luai_userstateresume
#undef luai_userstateresume
#endif
#define luai_userstateresume(L, n) SGEGameResumeLuaTask(L, n)

#ifdef luai_userstateyield
#undef luai_userstateyield
#endif
#define luai_userstateyield(L, n) SGEGameYieldLuaTask(L, n)

#endif /* SGE_LUA_H */
