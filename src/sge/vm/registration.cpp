//
//
#include <sge/vm/registration.hpp>

SGE_VM_BEGIN

registration::registration(lua_State *L):
    m_L(L),
    m_beginning(false),
    m_new_begin(false)
{
}

registration::~registration(void)
{
}

void registration::function(lua_State *L, const char *name, lua_CFunction fp)
{
    lua_pushcfunction(L, fp);
    lua_setfield(L, -2, name);
}

bool registration::userdata_construct(lua_State *L, const rttr::type &t);


void registration::begin(void)
{
    lua_newtable(m_L);
}

void registration::end(void)
{
    lua_setglobal(m_L, "sge");
}

SGE_VM_END
