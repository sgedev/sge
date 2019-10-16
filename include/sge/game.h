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
	uv_timer_t sleepTimer;
	void *data;
} SGE_Game_Task_t;

void SGE_Game_InitLua(lua_State* L);
void SGE_Game_ShutdownLua(lua_State* L);
void SGE_Game_AddLuaTask(lua_State* L, lua_State* L1);
void SGE_Game_RemoveLuaTask(lua_State* L, lua_State* L1);
void SGE_Game_ResumeLuaTask(lua_State* L, int n);
void SGE_Game_YieldLuaTask(lua_State* L, int n);

#ifdef __cplusplus
}
#endif

#ifdef LUA_EXTRASPACE
#undef LUA_EXTRASPACE
#endif
#define LUA_EXTRASPACE sizeof(SGE_Game_Task_t)

#if defined(luai_userstateopen)
#undef luai_userstateopen
#endif
#define luai_userstateopen SGE_Game_InitLua

#if defined(luai_userstateclose)
#undef luai_userstateclose
#endif
#define luai_userstateclose SGE_Game_ShutdownLua

#if defined(luai_userstatethread)
#undef luai_userstatethread
#endif
#define luai_userstatethread SGE_Game_AddLuaTask

#if defined(luai_userstatefree)
#undef luai_userstatefree
#endif
#define luai_userstatefree SGE_Game_RemoveLuaTask

#if defined(luai_userstateresume)
#undef luai_userstateresume
#endif
#define luai_userstateresume SGE_Game_ResumeLuaTask

#if defined(luai_userstateyield)
#undef luai_userstateyield
#endif
#define luai_userstateyield SGE_Game_YieldLuaTask

#endif /* SGE_GAME_H */

