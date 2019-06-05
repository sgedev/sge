/*
 *
 */
#ifndef SGE_GAME_LUA_USER_H
#define SGE_GAME_LUA_USER_H

#include <uv.h>
#include <xlist.h>

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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	xlist_node_t node;
	xlist_t wait_list;
	uv_timer_t sleep_timer;
	void *data;
} sge_game_vm_task_t;

#define LUA_EXTRASPACE sizeof(sge_game_vm_task_t)

extern void sge_game_vm_open(lua_State *L);
extern void sge_game_vm_close(lua_State *L);
extern void sge_game_vm_thread(lua_State *L, lua_State *L1);
extern void sge_game_vm_free(lua_State *L, lua_State *L1);
extern void sge_game_vm_resume(lua_State *L, int n);
extern void sge_game_vm_yield(lua_State *L, int n);

#define luai_userstateopen(L) sge_game_vm_open(L)
#define luai_userstateclose(L) sge_game_vm_close(L)
#define luai_userstatethread(L, L1) sge_game_vm_thread(L, L1)
#define luai_userstatefree(L, L1) sge_game_vm_free(L, L1)
#define luai_userstateresume(L, n) sge_game_vm_resume(L, n)
#define luai_userstateyield(L, n) sge_game_vm_yield(L, n)

#ifdef __cplusplus
}
#endif

#endif /* SGE_GAME_LUA_USER_H */

