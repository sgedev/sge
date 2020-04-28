
sge_sources(
	common.h
	config.h.in
	scene.h
	vm.h
	vm_lua.h
	)

configure_file(
	config.h.in
	config.h
	)

sge_compile_definitions(LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/vm_lua.h\")

