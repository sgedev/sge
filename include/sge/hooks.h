/*
 *
 */
#ifndef SGE_HOOKS_H
#define SGE_HOOKS_H

#include <uv.h>
#include <cx/list.h>

 /*
  *
  */
#ifndef SGE_VM_HOOKS_H
#define SGE_VM_HOOKS_H

#include <uv.h>
#include <cx/list.h>

#undef LUA_EXTRASPACE
#undef luai_userstateopen
#undef luai_userstateclose
#undef luai_userstatethread
#undef luai_userstatefree
#undef luai_userstateresume
#undef luai_userstateyield

CX_BEGIN_DECLS

#define SGE_TASK_PRIORITY_MAX 8

typedef struct sge_object_struct sge_object_t;

typedef struct {
	uv_loop_t* loop;
	uv_timer_t sleep_timer;
	int priority;
	cx_list_node_t node;
	cx_list_t wait_task_list;
	sge_object_t* object;
} sge_task_t;

void sge_kernel_open(lua_State* L);
void sge_kernel_close(lua_State* L);
void sge_kernel_thread(lua_State* L, lua_State* T);
void sge_kernel_free(lua_State* L, lua_State* T);
void sge_kernel_resume(lua_State* T, int n);
void sge_kernel_yeild(lua_State* T, int n);

CX_END_DECLS

#define LUA_EXTRASPACE sizeof(sge_task_t)
#define luai_userstateopen(L) sge_kernel_open(L)
#define luai_userstateclose(L) sge_kernel_close(L)
#define luai_userstatethread(L, T) sge_kernel_thread(L, T)
#define luai_userstatefree(L, T) sge_kernel_free(L, T)
#define luai_userstateresume(T, n) sge_kernel_resume(T, n)
#define luai_userstateyield(T, n) sge_kernel_yeild(T, n)

#endif /* SGE_HOOKS_H */


#endif /* SGE_HOOKS_H */
