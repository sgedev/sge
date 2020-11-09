//
//
#include <sge/lua/registration.hpp>

SGE_LUA_BEGIN

registration::registration(lua_State *L):
    m_L(L)
{
    SGE_ASSERT(m_L != nullptr);

}

registration::~registration(void)
{
    SGE_ASSERT(m_table_stack.empty());

    lua_pop(m_L, 1);
}

registration &registration::table(const char *name)
{
    SGE_ASSERT(name != nullptr);

    lua_newtable(m_L);
    m_table_stack.push(name);
}

registration &registration::def(const char *name, bool v)
{
    SGE_ASSERT(name != nullptr);

    lua_pushboolean(m_L, v);

    if (m_table_stack)
}

registration &registration::def(const char *name, int v)
{
    SGE_ASSERT(name != nullptr);
}

registration &registration::def(const char *name, double v)
{
    SGE_ASSERT(name != nullptr);
}

registration &registration::def(const char *name, const char *p)
{
    SGE_ASSERT(name != nullptr);
    SGE_ASSERT(p != nullptr);
}

registration &registration::def(const char *name, rttr::type type)
{
    SGE_ASSERT(name != nullptr);
    SGE_ASSERT(type.is_class());


}

SGE_LUA_END
