
sge_sources(
	camera.h
	common.h
	config.h.in
	input.h
	scene.h
	vm.h
	vm_lua.h
	window.h
	)

configure_file(
	config.h.in
	config.h
	)

sge_compile_definitions(LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/vm_lua.h\")

