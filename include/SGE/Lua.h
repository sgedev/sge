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

void sgeLuaInit(lua_State *L);
void sgeLuaShutdown(lua_State *L);
void sgeLuaTaskNew(lua_State *L, lua_State *L1);
void sgeLuaTaskFree(lua_State *L, lua_State *L1);
void sgeLuaTaskResume(lua_State *L, int n);
void sgeLuaTaskYield(lua_State *L, int n);

#ifdef __cplusplus
}
#endif

#define LUA_EXTRASPACE sizeof(SGELuaTask)

#define luai_userstateopen(L) sgeLuaInit(L)
#define luai_userstateclose(L) sgeLuaShutdown(L)
#define luai_userstatethread(L, L1) sgeLuaTaskNew(L, L1)
#define luai_userstatefree(L, L1) sgeLuaTaskFree(L, L1)
#define luai_userstateresume(L, n) sgeLuaTaskResume(L, n)
#define luai_userstateyield(L, n) sgeLuaTaskYield(L, n)

#endif /* SGE_LUA_H */
