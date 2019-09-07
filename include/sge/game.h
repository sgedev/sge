/*
 *
 */
#ifndef SGE_GAME_H
#define SGE_GAME_H

#include <uv.h>

#ifdef LoadString
#undef LoadString
#endif

#ifdef __cplusplus
extern "C" {
#endif

void sge_game_lua_open(lua_State *L);
void sge_game_lua_close(lua_State *L);
void sge_game_lua_thread(lua_State *L, lua_State *L1);
void sge_game_lua_free(lua_State *L, lua_State *L1);
void sge_game_lua_resume(lua_State *L, int n);
void sge_game_lua_yield(lua_State *L, int n);

#ifdef __cplusplus
}
#endif

#ifdef LUA_EXTRASPACE
#undef LUA_EXTRASPACE
#endif
#define LUA_EXTRASPACE sizeof(void *)

#if defined(luai_userstateopen)
#undef luai_userstateopen
#endif
#define luai_userstateopen sge_game_lua_open

#if defined(luai_userstateclose)
#undef luai_userstateclose
#endif
#define luai_userstateclose sge_game_lua_close

#if defined(luai_userstatethread)
#undef luai_userstatethread
#endif
#define luai_userstatethread sge_game_lua_thread

#if defined(luai_userstatefree)
#undef luai_userstatefree
#endif
#define luai_userstatefree sge_game_lua_free

#if defined(luai_userstateresume)
#undef luai_userstateresume
#endif
#define luai_userstateresume sge_game_lua_resume

#if defined(luai_userstateyield)
#undef luai_userstateyield
#endif
#define luai_userstateyield sge_game_lua_yield

#endif /* SGE_GAME_H */

