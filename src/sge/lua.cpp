//
//
#include <vector>

#include <sge/lua.hpp>

SGE_LUA_BEGIN

struct nil { };
struct function { };
struct table { };
struct thread { };
struct userdata { };

typedef std::vector<rttr::argument> args;

template <typename T>
struct ref {
    int id;

    ref(void):
        id(LUA_REFNIL)
    {
    }

    ref(const ref &that):
        id(that.id)
    {
    }

    bool create(lua_State *L)
    {
        id = luaL_ref(L, LUA_REGISTRYINDEX);
        return (id != LUA_REFNIL);
    }

    void get(lua_State *L)
    {
        lua_rawgeti(L, LUA_REGISTRYINDEX, id);
    }

    ref &operator=(const ref &that)
    {
        if (this != &that)
            id = that.id;
        return (*this);
    }
};

struct function_context {
    rttr::type type;
    rttr::method method;
};

struct property_context {
    rttr::type type;
    rttr::property property;
};

template <typename T>
rttr::variant make_ref(lua_State *L, int i)
{
    ref<T> v;

    lua_pushvalue(L, i);
    if (!v.create(L))
        return rttr::variant();

    return rttr::variant(v);
}

template <>
rttr::variant make_ref<userdata>(lua_State *L, int i)
{
    SGE_ASSERT(lua_isuserdata(L, i));

    if (!lua_getmetatable(L, i))
        return false;

    lua_pushstring(L, "__name");
    lua_rawget(L, 2);
    auto type = rttr::type::get_by_name(lua_tostring(L, -1));
    lua_pop(L, 1);

    if (!type)
        return rttr::variant();

    return *reinterpret_cast<rttr::variant *>(lua_touserdata(L, i));
}

template <typename T>
void push_ref(lua_State *L, const rttr::variant &v)
{

}

static inline rttr::variant tovariant(lua_State *L, int i)
{
    int type = lua_type(L, i);

    if (type == LUA_TBOOLEAN)
        return rttr::variant(!!lua_toboolean(L, i));

    if (type == LUA_TNUMBER)
        return rttr::variant(lua_tonumber(L, i));

    if (type == LUA_TSTRING)
        return rttr::variant(std::string(lua_tostring(L, i)));

    if (type == LUA_TLIGHTUSERDATA)
        return rttr::variant(lua_touserdata(L, i));

    if (type == LUA_TFUNCTION)
        return make_ref<function>(L, i);

    if (type == LUA_TTABLE)
        return make_ref<table>(L, i);

    if (type == LUA_TTHREAD)
        return make_ref<thread>(L, i);

    if (type == LUA_TUSERDATA)
        return make_ref<userdata>(L, i);

    if (type == LUA_TNIL)
        return rttr::variant(nil());

    return rttr::variant();
}

static inline int pushvariant(lua_State *L, rttr::variant &v)
{
    if (!v) {
        lua_pushnil(L);
        return 1;
    }

    auto type = v.get_type();

    if (type == rttr::type::get<bool>()) {
        lua_pushboolean(L, v.get_value<bool>());
        return 1;
    }

    if (type == rttr::type::get<int>()) {
        lua_pushinteger(L, v.get_value<int>());
        return 1;
    }

    if (type == rttr::type::get<float>() || type == rttr::type::get<double>()) {
        lua_pushnumber(L, v.get_value<double>());
        return 1;
    }

    if (type == rttr::type::get<std::string>()) {
        lua_pushstring(L, v.get_value<std::string>().c_str());
        return 1;
    }

    if (v.can_convert<nil>()) {
        lua_pushnil(L);
        return 1;
    }

    if (v.can_convert<ref>()) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, v.get_value<function_ref>().ref);
        return 1;
    }

    if (v.can_convert<table_ref>()) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, v.get_value<table_ref>().ref);
        return 1;
    }

    if (v.can_convert<thread_ref>()) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, v.get_value<thread_ref>().ref);
        return 1;
    }

    auto type_name = type.get_name();

    return 0;
}

static inline rttr::variant invoke_variadic(rttr::method &method, rttr::variant &inst, lua_State *L, int first_arg)
{
    lua_args_t largs;
    int i = first_arg;


    return rttr::variant();
}

static int method_invoker(lua_State *L)
{
    int top = lua_gettop(L);
    if (top < 1)
        luaL_error(L, "fatal.");

    auto lm = (lua_method_t *)lua_touserdata(L, lua_upvalueindex(1));
    if (lm == nullptr)
        luaL_error(L, "fatal.");

    rttr::variant ret;
    auto ud = (rttr::variant *)checkudata(L, 1, lm->type);
    SGE_ASSERT(ud != nullptr);

    int n = lua_gettop(L) - 1;
    switch (n) {
    case 0:
        ret = lm->method.invoke(*ud);
        break;
    case 1:
        ret = lm->method.invoke(*ud, tovariant(L, 2));
        break;
    case 2:
        ret = lm->method.invoke(*ud, tovariant(L, 2), tovariant(L, 3));
        break;
    case 3:
        ret = lm->method.invoke(*ud, tovariant(L, 2), tovariant(L, 3), tovariant(L, 4));
        break;
    case 4:
        ret = lm->method.invoke(*ud, tovariant(L, 2), tovariant(L, 3), tovariant(L, 4), tovariant(L, 5));
        break;
    case 5:
        ret = lm->method.invoke(*ud, tovariant(L, 2), tovariant(L, 3), tovariant(L, 4), tovariant(L, 5), tovariant(L, 6));
        break;
    case 6:
        ret = lm->method.invoke(*ud, tovariant(L, 2), tovariant(L, 3), tovariant(L, 4), tovariant(L, 5), tovariant(L, 6), tovariant(L, 7));
        break;
    default:
        ret = invoke_variadic(lm->method, *ud, L, 2);
        break;
    }

    return pushvariant(L, ret);
}

static void register_fields(lua_State *L, int i, const rttr::type &type)
{
    getmetatable(L, type);

    auto methods = type.get_methods();
    for (auto method: methods) {
        if (method.get_access_level() != rttr::access_levels::public_access)
            continue;
        auto lm = (lua_method_t *)lua_newuserdata(L, sizeof(lua_method_t));
        lm->type = type;
        lm->method = method;
        lua_pushcclosure(L, &method_invoker, 1);
        lua_setfield(L, -2, method.get_name().data());
    }

    auto properties = type.get_properties();
    for (auto property: methods) {
        if (property.get_access_level() != rttr::access_levels::public_access)
            continue;
    }

    lua_setmetatable(L, i);
}

bool create(lua_State *L, const rttr::type &type)
{
    SGE_ASSERT(type);

    lua_args_t largs;

    int n = lua_gettop(L);
    for (int i = 1; i <= n; ++i) {
        rttr::variant v(tovariant(L, i));
        if (v)
            largs.emplace_back(v);
    }

    auto pv = (rttr::variant *)lua_newuserdata(L, sizeof(rttr::variant));

    auto ctors = type.get_constructors();
    for (auto ctor: ctors) {
        auto pinfos = ctor.get_parameter_infos();
        if (pinfos.size() != largs.size())
            continue;
        size_t li = 0;
        size_t matchs = 0;
        for (auto pi(pinfos.begin()); pi != pinfos.end(); ++pi) {
            if (pi->get_type() != largs[li].get_type())
                break;
            matchs += 1;
        }
        if (matchs == pinfos.size()) {
            *pv = ctor.invoke_variadic(largs);
            register_fields(L, -1, type);
            return true;
        }
    }

    return false;
}

SGE_LUA_END
