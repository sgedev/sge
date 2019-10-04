/*
 *
 */
#ifndef SGE_GAME_H
#define SGE_GAME_H

#include <uv.h>

#include <cx/list.h>

#ifdef LoadString
#undef LoadString
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	CXListNode node;
	uv_timer_t sleep_timer;
	void *data;
} SGEGameTask_t;

void SGEGameInitLua(lua_State* L);
void SGEGameShutdownLua(lua_State* L);
void SGEGameAddLuaTask(lua_State* L, lua_State* L1);
void SGEGameRemoveLuaTask(lua_State* L, lua_State* L1);
void SGEGameResumeLuaTask(lua_State* L, int n);
void SGEGameYieldLuaTask(lua_State* L, int n);

#ifdef __cplusplus
}
#endif

#ifdef LUA_EXTRASPACE
#undef LUA_EXTRASPACE
#endif
#define LUA_EXTRASPACE sizeof(SGEGameTask_t)

#if defined(luai_userstateopen)
#undef luai_userstateopen
#endif
#define luai_userstateopen SGEGameInitLua

#if defined(luai_userstateclose)
#undef luai_userstateclose
#endif
#define luai_userstateclose SGEGameShutdownLua

#if defined(luai_userstatethread)
#undef luai_userstatethread
#endif
#define luai_userstatethread SGEGameAddLuaTask

#if defined(luai_userstatefree)
#undef luai_userstatefree
#endif
#define luai_userstatefree SGEGameRemoveLuaTask

#if defined(luai_userstateresume)
#undef luai_userstateresume
#endif
#define luai_userstateresume SGEGameResumeLuaTask

#if defined(luai_userstateyield)
#undef luai_userstateyield
#endif
#define luai_userstateyield SGEGameYieldLuaTask

#endif /* SGE_GAME_H */

