//
//
#ifndef SGE_LUA_HPP
#define SGE_LUA_HPP

#include <lua.hpp>

#include <SGE/Common.hpp>

#define SGE_LUA_BEGIN SGE_BEGIN namespace Lua {
#define SGE_LUA_END } SGE_END

SGE_LUA_BEGIN

static SGE_INLINE void push(lua_State *L, bool v)
{
	lua_pushboolean(L, v);
}

static SGE_INLINE void push(lua_State *L, int v)
{
	lua_pushinteger(L, v);
}

static SGE_INLINE void push(lua_State *L, double v)
{
	lua_pushnumber(L, v);
}

static SGE_INLINE void push(lua_State *L, const glm::vec2 &v)
{

}

static SGE_INLINE void push(lua_State *L, const glm::vec3 &v)
{

}

static SGE_INLINE void push(lua_State *L, const glm::vec4 &v)
{

}

static SGE_INLINE void push(lua_State *L, const glm::mat4 &v)
{

}

static SGE_INLINE void check(lua_State *L, const glm::vec2 &v)
{

}

static SGE_INLINE void check(lua_State *L, const glm::vec3 &v)
{

}

static SGE_INLINE void check(lua_State *L, const glm::vec4 &v)
{

}

SGE_LUA_END

#endif // SGE_LUA_HPP
