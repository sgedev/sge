//
//
#ifndef SGE_LUA_REF_HPP
#define SGE_LUA_REF_HPP

#include <sge/lua/common.hpp>

SGE_LUA_BEGIN

class ref final {
public:
    ref(void);
    ref(const ref &that);

public:
    bool create(lua_State *L);
    void get(lua_State *L);
    bool is_valid(void) const;
    operator bool(void) const;
    ref &operator=(const ref &that);

private:
    int m_value;
};

SGE_INLINE ref::ref(void):
    m_value(LUA_NOREF)
{
}

SGE_INLINE ref::ref(const ref &that):
    m_value(that.m_value)
{
}

SGE_INLINE bool ref::create(lua_State *L)
{
    SGE_ASSERT(m_value == LUA_NOREF);
    m_value = luaL_ref(L, LUA_REGISTRYINDEX);
    return (m_value != LUA_NOREF);
}

SGE_INLINE void ref::get(lua_State *L)
{
    SGE_ASSERT(m_value != LUA_NOREF);
    lua_rawgeti(L, LUA_REGISTRYINDEX, m_value);
}

SGE_INLINE bool ref::is_valid(void) const
{
    return (m_value != LUA_NOREF);
}

SGE_INLINE ref::operator bool(void) const
{
    return is_valid();
}

SGE_INLINE ref &ref::operator=(const ref &that)
{
    if (this != &that)
        m_value = that.m_value;
    return (*this);
}

SGE_LUA_END

#endif // SGE_LUA_REF_HPP
