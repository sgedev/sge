/*
 *
 */
#ifndef LUASGE_H
#define LUASGE_H

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

#include <uv.h>
#include <cx/list.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	cx_list_node_t node;
	uv_timer_t sleep_timer;
} sge_task_t;

extern void sge_init(lua_State *L);
extern void sge_shutdown(lua_State *L);
extern void sge_task_new(lua_State *L, lua_State *L1);
extern void sge_task_destroy(lua_State *L, lua_State *L1);
extern void sge_task_resume(lua_State *L, int n);
extern void sge_task_yield(lua_State *L, int n);

#ifdef __cplusplus
}
#endif

#define LUA_EXTRASPACE sizeof(sge_task_t)

#define luai_userstateopen(L) sge_init(L)
#define luai_userstateclose(L) sge_shutdown(L)
#define luai_userstatethread(L, L1) sge_task_new(L, L1)
#define luai_userstatefree(L, L1) sge_task_destroy(L, L1)
#define luai_userstateresume(L, n) sge_task_resume(L, n)
#define luai_userstateyield(L, n) sge_task_yield(L, n)

#endif /* LUASGE_H */
