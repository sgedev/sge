/*
 *
 */
#ifndef SGE_VM_LUA_H
#define SGE_VM_LUA_H

#include <uv.h>
#include <physfs.h>
#include <cx/list.h>

/*
** Hooks
*/

#ifdef LUA_EXTRASPACE
#   undef LUA_EXTRASPACE
#endif

#ifndef luai_userstateopen
#   undef luai_userstateopen
#endif

#ifndef luai_userstateclose
#   undef luai_userstateclose
#endif

#ifndef luai_userstatethread
#   undef luai_userstatethread
#endif

#ifndef luai_userstatefree
#   undef luai_userstatefree
#endif

#ifndef luai_userstateresume
#   undef luai_userstateresume
#endif

#ifndef luai_userstateyield
#   undef luai_userstateyield
#endif

#ifdef _WIN32
#   undef LoadString
#endif

typedef struct {
    cx_list_node_t node;
    uv_timer_t sleep_timer;
} sge_vm_task_t;

void sge_vm_start(lua_State *L);
void sge_vm_stop(lua_State *L);
void sge_vm_task_add(lua_State *L, lua_State *L1);
void sge_vm_task_remove(lua_State *L, lua_State *L1);
void sge_vm_task_resume(lua_State *L, int n);
void sge_vm_task_yield(lua_State *L, int n);

#define LUA_EXTRASPACE sizeof(sge_vm_task_t)

#define luai_userstateopen(L) sge_vm_start(L)
#define luai_userstateclose(L) sge_vm_stop(L)
#define luai_userstatethread(L, L1) sge_vm_task_add(L, L1)
#define luai_userstatefree(L, L1) sge_vm_task_remove(L, L1)
#define luai_userstateresume(L, n) sge_vm_task_resume(L, n)
#define luai_userstateyield(L, n) sge_vm_task_yield(L, n)

/*
** I/O
*/

#define LUA_PHYSFS

#ifdef LUA_FILE
#   undef LUA_FILE
#endif

#ifdef luai_fopen
#   undef luai_fopen
#endif

#ifdef luai_freopen
#   undef luai_freopen
#endif

#ifdef luai_feof
#   undef luai_feof
#endif

#ifdef luai_fread
#   undef luai_fread
#endif

#ifdef luai_getc
#   undef luai_getc
#endif

#define LUA_FILE PHYSFS_File

LUA_FILE *sge_vm_io_fopen(const char *filename, const char *mode);
LUA_FILE *sge_vm_io_freopen(const char *filename, const char *mode, LUA_FILE **fp);
int sge_vm_io_fclose(LUA_FILE *fp);
int sge_vm_io_feof(LUA_FILE *fp);
size_t sge_vm_io_fread(void *buf, int n, int size, LUA_FILE *fp);
int sge_vm_io_getc(LUA_FILE *fp);
int sge_vm_io_ferror(LUA_FILE *fp);

#define luai_fopen(fn, m) sge_vm_io_fopen(fn, m)
#define luai_freopen(fn, m, f) sge_vm_io_freopen(fn, m, &(f))
#define luai_fclose(f) sge_vm_io_fclose(f)
#define luai_feof(f) sge_vm_io_feof(f)
#define luai_fread(b, n, s, f) sge_vm_io_fread(b, n, s, f)
#define luai_getc(f) sge_vm_io_getc(f)
#define luai_ferror(f) sge_vm_io_ferror(f)

#endif /* SGE_VM_LUA_H */
