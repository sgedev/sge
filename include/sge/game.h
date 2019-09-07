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
	cx_list_node_t node;
	uv_timer_t sleep_timer;
	void *data;
} sge_game_task;

void sge_game_luaopen(lua_State* L);
void sge_game_luaclose(lua_State* L);
void sge_game_luathread(lua_State* L, lua_State* L1);
void sge_game_luafree(lua_State* L, lua_State* L1);
void sge_game_luaresume(lua_State* L, int n);
void sge_game_luayield(lua_State* L, int n);

#ifdef __cplusplus
}
#endif

#ifdef LUA_EXTRASPACE
#undef LUA_EXTRASPACE
#endif
#define LUA_EXTRASPACE sizeof(sge_game_task)

#if defined(luai_userstateopen)
#undef luai_userstateopen
#endif
#define luai_userstateopen sge_game_luaopen

#if defined(luai_userstateclose)
#undef luai_userstateclose
#endif
#define luai_userstateclose sge_game_luaclose

#if defined(luai_userstatethread)
#undef luai_userstatethread
#endif
#define luai_userstatethread sge_game_luathread

#if defined(luai_userstatefree)
#undef luai_userstatefree
#endif
#define luai_userstatefree sge_game_luafree

#if defined(luai_userstateresume)
#undef luai_userstateresume
#endif
#define luai_userstateresume sge_game_luaresume

#if defined(luai_userstateyield)
#undef luai_userstateyield
#endif
#define luai_userstateyield sge_game_luayield

#endif /* SGE_GAME_H */

