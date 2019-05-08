/*
 *
 */
#ifndef SGE_GAME_VM_LUA_USER_H
#define SGE_GAME_VM_LUA_USER_H

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

typedef struct sge_game_vm_task {
	xlist_node_t node;
	void *vm;
	uv_timer_t sleep_timer;
} sge_game_vm_task_t;

#define LUA_EXTRASPACE sizeof(sge_game_vm_task_t)

extern void sge_game_vm_init(lua_State *L);
extern void sge_game_vm_shutdown(lua_State *L);
extern void sge_game_vm_add_task(lua_State *L, lua_State *L1);
extern void sge_game_vm_remove_task(lua_State *L, lua_State *L1);
extern void sge_game_vm_resume_task(lua_State *L, int n);
extern void sge_game_vm_yield_task(lua_State *L, int n);

#define luai_userstateopen(L) sge_game_vm_init(L)
#define luai_userstateclose(L) sge_game_vm_shutdown(L)
#define luai_userstatethread(L, L1) sge_game_vm_add_task(L, L1)
#define luai_userstatefree(L, L1) sge_game_vm_remove_task(L, L1)
#define luai_userstateresume(L, n) sge_game_vm_resume_task(L, n)
#define luai_userstateyield(L, n) sge_game_vm_yield_task(L, n)

#ifdef __cplusplus
}
#endif

#endif /* SGE_GAME_VM_LUA_USER_H */

