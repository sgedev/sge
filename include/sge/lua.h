/*
 *
 */
#ifndef SGE_LUA_H
#define SGE_LUA_H

#include <cx/list.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	CXListNode node;
	void *vm;
	void *task;
} SGEVMContext;

void SGEVMInitLua(lua_State *L);
void SGEVMShutdownLua(lua_State *L);
void SGEVMAddLuaTask(lua_State *L, lua_State *L1);
void SGEVMRemoveLuaTask(lua_State *L, lua_State *L1);
void SGEVMResumeLuaTask(lua_State *L, int n);
void SGEVMYieldLuaTask(lua_State *L, int n);

#ifdef __cplusplus
}
#endif

#ifdef LUA_EXTRASPACE
#undef LUA_EXTRASPACE
#endif
#define LUA_EXTRASPACE sizeof(SGEVMContext)

#ifdef luai_userstateopen
#undef luai_userstateopen
#endif
#define luai_userstateopen(L) SGEVMInitLua(L)

#ifdef luai_userstateclose
#undef luai_userstateclose
#endif
#define luai_userstateclose(L) SGEVMShutdownLua(L)

#ifdef luai_userstatethread
#undef luai_userstatethread
#endif
#define luai_userstatethread(L, L1) SGEVMAddLuaTask(L, L1)

#ifdef luai_userstatefree
#undef luai_userstatefree
#endif
#define luai_userstatefree(L, L1) SGEVMRemoveLuaTask(L, L1)

#ifdef luai_userstateresume
#undef luai_userstateresume
#endif
#define luai_userstateresume(L, n) SGEVMResumeLuaTask(L, n)

#ifdef luai_userstateyield
#undef luai_userstateyield
#endif
#define luai_userstateyield(L, n) SGEVMYieldLuaTask(L, n)

#endif /* SGE_LUA_H */
