//
//
#ifndef SGE_LUA_REGISTRATION_HPP
#define SGE_LUA_REGISTRATION_HPP

#include <string>
#include <stack>

#include <sge/lua/common.hpp>

SGE_LUA_BEGIN

class registration {
public:
    registration(lua_State *L);
    ~registration(void);

public:
    registration &table(const char *name);
    registration &def(const char *name, bool v);
    registration &def(const char *name, int v);
    registration &def(const char *name, double v);
    registration &def(const char *name, const char *p);
    registration &def(const char *name, rttr::type t);

private:
    lua_State *m_L;
    std::stack<std::string> m_table_stack;
};

    void init(lua_State *L);
    void shutdown(void);
    void table(const char *name);
    void def(const char *name, bool v);
    void def(const char *name, int v);
    void def(const char *name, double v);
    void def(const char *name, const char *p);
    void def(const char *name, rttr::type type);
}

SGE_LUA_END

#endif // SGE_LUA_REGISTRATION_HPP
