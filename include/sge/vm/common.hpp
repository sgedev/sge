//
//
#ifndef SGE_VM_COMMON_HPP
#define SGE_VM_COMMON_HPP

#include <lua.hpp>

#include <sge/common.hpp>

#define SGE_VM_BEGIN SGE_BEGIN namespace vm {
#define SGE_VM_END } SGE_END

SGE_VM_BEGIN

namespace lua_registration {
	static inline void begin(lua_State *L)
	{
		lua_newtable(L);
	}

	static inline void end(lua_State *L)
	{
		lua_setglobal(L, "sge");
	}

	static inline void function(lua_State *L, const char *name, lua_CFunction fp)
	{
		lua_pushcfunction(L, fp);
		lua_setfield(L, -2, name);
	}

	template <typename T>
	const char *metatable_name(void)
	{
		rttr::type::get<T>().get_name().data();
	}

	template <typename T>
	void metatable_new(void)
	{
		luaL_newmetatable(L, metatable_name<T>());
	}

	template <typename T>
	void metatable_get(void)
	{
		luaL_getmetatable(L, metatable_name<T>());
	}

	template <typename T>
	T *checkuserdata(lua_State *L, int i)
	{
		return reinterpret_cast<T *>(luaL_checkudata(L, i, metatable_name<T>()));
	}

	void userdata_construct(lua_State *L, const rttr::type &t);

	template <typename T, typename M>
	int userdata_new(lua_State *L)
	{
		T *obj = reinterpret_cast<T *>(lua_newuserdata(L, sizeof(T)));
		if (obj == nullptr) {
			lua_pusnil(L);
			return 1;
		}

		userdata_construct(L, rttr::type::get<T>());

		metatable_get<T>();
		lua_setmetatable(L, -2);

		return 1;
	}

	template <typename T>
	int userdata_method(lua_State *L)
	{
		T *obj = checkudata<T>(L, 1);
		return 0;
	}

	template <typename T>
	void userdata(lua_State *L, const char *name)
	{
		lua_pushstring(L, name);
		lua_pushcfunction(L, &userdata_new<T>);
		lua_rawset(L, -3);

		metatable_new<T>();

		auto type = rttr::type::get<T>();
		auto methods = type.get_methods();
		for (auto method: methods) {
			lua_pushstring(L, method.get_name().data());
			//lua_pushcfunction(L, );
			lua_rawset(L, -3);
		}

		lua_pop(L, 1);
	}
}

SGE_VM_END

#endif // SGE_VM_COMMON_HPP
