//
//
#ifndef SGE_VM_REGISTRATION_HPP
#define SGE_VM_REGISTRATION_HPP

#include <functional>

#include <sge/vm/common.hpp>

SGE_VM_BEGIN

class registration {
public:
    registration(lua_State *L);
    virtual ~registration(void);

public:
    void add(const char *name, lua_CFunction fn);
    void add(const char *name, std::function<int (lua_State *)> fn);
    bool add(const char *name, const rttr::type &t);

protected:
    template <typename T>
    const char *metatable_name(void)
    {
        return rttr::type::get<T>().get_name().data();
    }

    template <typename T>
    void metatable_new(lua_State *L)
    {
        luaL_newmetatable(L, metatable_name<T>());
    }

    template <typename T>
    void metatable_get(lua_State *L)
    {
        luaL_getmetatable(L, metatable_name<T>());
    }

    template <typename T>
    T *check_udata(lua_State *L, int i)
    {
        return reinterpret_cast<T *>(luaL_checkudata(L, i, metatable_name<T>()));
    }

    template <typename T>
    int userdata_new(lua_State *L)
    {
        T *obj = reinterpret_cast<T *>(lua_newuserdata(L, sizeof(T)));
        if (obj == nullptr) {
            lua_pushnil(L);
            return 1;
        }

        userdata_construct(L, rttr::type::get<T>());

        metatable_get<T>(L);
        lua_setmetatable(L, -2);

        return 1;
    }

    template <typename T>
    int userdata_method(lua_State *L)
    {
        T *obj = check_udata<T>(L, 1);
        return 0;
    }

    template <typename T>
    void userdata(lua_State *L, const char *name)
    {
        lua_pushstring(L, name);
        lua_pushcfunction(L, [](lua_State *L) -> int { return userdata_new<T>(L); });
        lua_rawset(L, -3);

        metatable_new<T>(L);

        auto type = rttr::type::get<T>();
        auto methods = type.get_methods();
        for (auto method: methods) {
            lua_pushstring(L, method.get_name().data());
            //lua_pushcfunction(L, );
            lua_rawset(L, -3);
        }

        lua_pop(L, 1);
    }

protected:
    void begin(void);
    void end(void);

private:
    lua_State *m_L;
    bool m_beginning;
    bool m_new_begin;
};

SGE_VM_END

#endif // SGE_VM_REGISTRATION_HPP
