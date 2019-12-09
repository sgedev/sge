/*
 *
 */
#ifndef SGE_GAME_H
#define SGE_GAME_H

#include <uv.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LUA_EXTRASPACE
#undef LUA_EXTRASPACE
#endif

#ifdef luai_userstateopen
#undef luai_userstateopen
#endif

#ifdef luai_userstateclose
#undef luai_userstateclose
#endif

#ifdef luai_userstatethread
#undef luai_userstatethread
#endif

#ifdef luai_userstatefree
#undef luai_userstatefree
#endif

#ifdef luai_userstateresume
#undef luai_userstateresume
#endif

#ifdef luai_userstateyield
#undef luai_userstateyield
#endif

typedef struct {
	uv_timer_t sleepTimer;
} sge_Game;

void sge_Game_luaOpen(lua_State *L);
void sge_Game_luaClose(lua_State *L);
void sge_Game_luaThread(lua_State *L, lua_State *L1);
void sge_Game_luaFree(lua_State *L, lua_State *L1);
void sge_Game_luaResume(lua_State *L, int n);
void sge_Game_luaYield(lua_State *L, int n);

#define LUA_EXTRASPACE sizeof(sge_game_t)

#define luai_userstateopen(L)		sge_Game_luaOpen(L)
#define luai_userstateclose(L)		sge_Game_luaClose(L)
#define luai_userstatethread(L,L1)	sge_Game_luaThread(L, L1)
#define luai_userstatefree(L,L1)	sge_Game_luaFree(L, L1)
#define luai_userstateresume(L,n)	sge_Game_luaResume(L, n)
#define luai_userstateyield(L,n)	sge_Game_luaYeild(L, n)

#ifdef __cplusplus
}
#endif

#endif /* SGE_GAME_H */

