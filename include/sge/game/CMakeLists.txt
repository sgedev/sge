
sge_compile_definitions(
	LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/vm_lua_user.h\"
	)

sge_sources(
	common.hpp
	vm.hpp
	vm_lua_user.h
	)

