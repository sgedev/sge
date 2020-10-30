//
//
#ifndef SGE_LUA_COMMON_HPP
#define SGE_LUA_COMMON_HPP

#include <string>

#include <lua.hpp>

#include <sge/common.hpp>

#define SGE_LUA_BEGIN SGE_BEGIN namespace lua {
#define SGE_LUA_END } SGE_END

SGE_LUA_BEGIN

SGE_INLINE static void push(lua_State *L, bool v)
{
    lua_pushboolean(L, v);
}

SGE_INLINE static void push(lua_State *L, float v)
{
    lua_pushnumber(L, v);
}

SGE_INLINE static void push(lua_State *L, double v)
{
    lua_pushnumber(L, v);
}

SGE_INLINE static void push(lua_State *L, int v)
{
    lua_pushinteger(L, v);
}

SGE_INLINE static void push(lua_State *L, const char *p)
{
    lua_pushstring(L, p);
}

SGE_INLINE static void push(lua_State *L, const std::string &r)
{
    lua_pushstring(L, r.c_str());
}

SGE_LUA_END

#endif // SGE_LUA_COMMON_HPP
