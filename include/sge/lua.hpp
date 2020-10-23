//
//
#ifndef SGE_LUA_HPP
#define SGE_LUA_HPP

#include <lua.hpp>

#include <sge/common.hpp>

#define SGE_LUA_BEGIN SGE_BEGIN namespace lua {
#define SGE_LUA_END } SGE_END

SGE_LUA_BEGIN

bool create(lua_State *L, const rttr::type &type);

template <typename T>
bool create(lua_State *L)
{
    return create(L, rttr::type::get<T>());
}

static inline const char *metatablename(const rttr::type &type)
{
    return type.get_name().data();
}

template <typename T>
const char *metatablename(void)
{
    return type_name(rttr::type::get<T>());
}

template <typename T>
void newmetatable(lua_State *L)
{
    luaL_newmetatable(L, metatablename<T>());
}

static inline void getmetatable(lua_State *L, const rttr::type &type)
{
    luaL_getmetatable(L, metatablename(type));
}

template <typename T>
void getmetatable(lua_State *L)
{
    getmetatable(L, rttr::type::get<T>());
}

void *checkudata(lua_State *L, int i, const rttr::type &type)
{
    return luaL_checkudata(L, i, metatablename(type));
}

template <typename T>
void *checkudata(lua_State *L, int i)
{
    return checkudata(L, i, rttr::type::get<T>());
}

template <typename T>
void pushconstructor(lua_State *L)
{
    lua_pushcfunction(L, [](lua_State *L) -> int {
        if (!create<T>(L))
            luaL_error(L, "failed to construct instance of '%s'.", metatablename<T>());
        return 1;
    });
}

SGE_LUA_END

#endif // SGE_LUA_HPP
