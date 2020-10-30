//
//
#ifndef SGE_LUA_USERDATA_HPP
#define SGE_LUA_USERDATA_HPP

#include <sge/lua/common.hpp>
#include <sge/lua/ref.hpp>

SGE_LUA_BEGIN

void userdata_pushtype(lua_State *L, rttr::type type);
bool userdata_constructor(lua_State *L, rttr::type type, rttr::instance instance);

template <typename T>
class userdata final {
public:
    userdata(void)
    {
    }

    userdata(const userdata &that):
        m_value(that.m_value)
    {
    }

public:
    bool construct(lua_State *L)
    {
        SGE_ASSERT(!m_value.is_valid());

        T *instance = reinterpret_cast<T *>(lua_newuserdata(L, type().get_sizeof()));
        if (instance == nullptr)
            return false;

        userdata_pushtype(L, type);
        lua_setmetatable(L, -2);

        if (!userdata_construct(L, type, instance)) {
            lua_pop(L, 1);
            return rttr::variant();
        }

        int ref = luaL_ref(L, LUA_REGISTRYINDEX);
        if (ref == LUA_NOREF) {
            lua_pop(L, 1);
            return rttr::variant();
        }

        return rttr::variant(ref);
    }

    void push(lua_State *L)
    {
        SGE_ASSERT(m_value.is_valid());
        SGE_ASSERT(m_value.can_convert<ref>());
        m_value.get_value<ref>().get(L);
    }

    T *tovalue(lua_State *L, int i)
    {
        SGE_ASSERT(m_value.is_valid());
        SGE_ASSERT(m_value.can_convert<ref>());
        return check(L, i);
    }

    const T *tovalue(lua_State *L, int i) const
    {
        SGE_ASSERT(m_value.is_valid());
        SGE_ASSERT(m_value.can_convert<ref>());
        return check(L, i);
    }

    userdata &operator=(const userdata &that)
    {
        if (this != &that)
            m_value = that.m_value;
        return *this;
    }

    static rttr::type type(void)
    {
        return rttr::type::get<T>();
    }

    static const char *type_name(void)
    {
        return type().get_name().data();
    }

    static T *check(lua_State *L, int i)
    {
        return reinterpret_cast<T *>(luaL_checkudata(L, i, type_name()));
    }

private:
    rttr::variant m_value;
};

SGE_LUA_END

#endif // SGE_LUA_REF_HPP
