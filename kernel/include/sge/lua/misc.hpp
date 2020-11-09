//
//
#ifndef SGE_LUA_MISC_HPP
#define SGE_LUA_MISC_HPP

#include <sge/lua/common.hpp>
#include <sge/lua/ref.hpp>

SGE_LUA_BEGIN

struct function {
};

struct table {
};

struct thread {
};

typedef ref<function> function_ref;
typedef ref<table> table_ref;
typedef ref<thread> thread_ref;

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

void push(lua_State *L, rttr::variant &v);

SGE_LUA_END

#endif // SGE_LUA_MISC_HPP
