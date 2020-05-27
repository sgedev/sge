/*
 *
 */
#ifndef SGE_LUA_H
#define SGE_LUA_H

#include <lua.h>
#include <lauxlib.h>

#include <sge/common.h>

SGE_BEGIN_C_DECLS

static SGE_INLINE void sge_lua_push_vec2(lua_State *L, const cx_vec2_t *p)
{
	lua_newtable(L);

	lua_pushnumber(L, p->x);
	lua_rawseti(L, -2, 0);

	lua_pushnumber(L, p->y);
	lua_rawseti(L, -2, 1);
}

static SGE_INLINE void sge_lua_push_vec3(lua_State *L, const cx_vec3_t *p)
{
	lua_newtable(L);

	lua_pushnumber(L, p->x);
	lua_rawseti(L, -2, 0);

	lua_pushnumber(L, p->y);
	lua_rawseti(L, -2, 1);

	lua_pushnumber(L, p->y);
	lua_rawseti(L, -2, 2);
}

static SGE_INLINE void sge_lua_push_quat(lua_State *L, const cx_quat_t *p)
{
	lua_newtable(L);

	lua_pushnumber(L, p->x);
	lua_rawseti(L, -2, 0);

	lua_pushnumber(L, p->y);
	lua_rawseti(L, -2, 1);

	lua_pushnumber(L, p->y);
	lua_rawseti(L, -2, 2);

	lua_pushnumber(L, p->w);
	lua_rawseti(L, -2, 3);
}

static SGE_INLINE void sge_lua_check_vec2(lua_State *L, int i, cx_vec2_t *p)
{
	luaL_argcheck(L, lua_istable(L, i), i, "vec2 expects a number table.");
	luaL_argcheck(L, lua_rawlen(L, i) == 2, i, "vec2 expects a number table.");

	lua_rawgeti(L, i, 0);
	p->x = luaL_checknumber(L, -1);
	lua_rawgeti(L, i, 1);
	p->y = luaL_checknumber(L, -1);

	lua_pop(L, 2);
}

static SGE_INLINE void sge_lua_check_vec3(lua_State *L, int i, cx_vec3_t *p)
{
	luaL_argcheck(L, lua_istable(L, i), i, "vec3 expects a number table.");
	luaL_argcheck(L, lua_rawlen(L, i) == 3, i, "vec3 expects a number table.");

	lua_rawgeti(L, i, 0);
	p->x = luaL_checknumber(L, -1);
	lua_rawgeti(L, i, 1);
	p->y = luaL_checknumber(L, -1);
	lua_rawgeti(L, i, 2);
	p->z = luaL_checknumber(L, -1);

	lua_pop(L, 3);
}

static SGE_INLINE void sge_lua_check_vec4(lua_State *L, int i, cx_vec4_t *p)
{
	luaL_argcheck(L, lua_istable(L, i), i, "vec4 expects a number table.");
	luaL_argcheck(L, lua_rawlen(L, i) == 4, i, "vec4 expects a number table.");

	lua_rawgeti(L, i, 0);
	p->x = luaL_checknumber(L, -1);
	lua_rawgeti(L, i, 1);
	p->y = luaL_checknumber(L, -1);
	lua_rawgeti(L, i, 2);
	p->z = luaL_checknumber(L, -1);
	lua_rawgeti(L, i, 3);
	p->w = luaL_checknumber(L, -1);

	lua_pop(L, 4);
}

static SGE_INLINE void sge_lua_check_quat(lua_State *L, int i, cx_quat_t *p)
{
	luaL_argcheck(L, lua_istable(L, i), i, "quat expects a number table.");
	luaL_argcheck(L, lua_rawlen(L, i) == 4, i, "quat expects a number table.");

	lua_rawgeti(L, i, 0);
	p->x = luaL_checknumber(L, -1);
	lua_rawgeti(L, i, 1);
	p->y = luaL_checknumber(L, -1);
	lua_rawgeti(L, i, 2);
	p->z = luaL_checknumber(L, -1);
	lua_rawgeti(L, i, 3);
	p->w = luaL_checknumber(L, -1);

	lua_pop(L, 4);
}

SGE_END_C_DECLS

#endif /* SGE_LUA_H */
