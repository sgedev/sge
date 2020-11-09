//
//
#include <sge/lua/userdata.hpp>

SGE_LUA_BEGIN

static int userdata_destroy(lua_State *L)
{
    return 0;
}

static void userdata_new_type(lua_State *L, rttr::type type)
{
    luaL_newmetatable(L, type.get_name().data());

    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, &userdata_destroy);
    lua_rawset(L, -3);
}

void userdata_pushtype(lua_State *L, rttr::type type)
{
    luaL_getmetatable(L, type.get_name().data());
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        userdata_new_type(L, type);
    }
}

bool userdata_construct(lua_State *L, rttr::type type, void *instance)
{
    return true;
}

SGE_LUA_END
