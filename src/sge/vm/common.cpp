//
//
#include <vector>

#include <sge/vm/common.hpp>

SGE_VM_BEGIN

namespace lua_registration {
	static bool 
	bool userdata_construct(lua_State *L, rttr::type &t)
	{
		auto ctors = t.get_constructors();
		if (!ctors.empty())
			return true;

		std::vector<rttr::argument> args;
		int argn = lua_gettop(L);

		for (int i = 1; i <= argn; ++i) {
			switch (lua_type(L, i)) {
			case LUA_TBOOLEAN:
				args.emplace_back(lua_toboolean(L, i));
				break;
			case LUA_TNUMBER:
				args.emplace_back(lua_tonumber(L, i));
				break;
			case LUA_TSTRING:
				args.emplace_back(lua_tostring(L, i));
				break;
			case LUA_TUSERDATA:
				break;
			}
		}

		for (auto ctor: ctors) {
			rttr::array_range<rttr::parameter_info> param_infos = ctor.get_parameter_infos();
			if (param_infos.size() != args.size())
				continue;

			int match = 0;
			auto pit = param_infos.cbegin();
			auto ait = args.cbegin();

			for (int i = 0; i < args.size(); ++i)
				match += ait->get_type().is_base_of(pit->get_type());

			if (match == args.size()) {
				ctor.invoke_variadic(args);
				return true;
			}
		}

		return false;
	}
}

SGE_VM_END
