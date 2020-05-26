
sge_subdirectories(
	renderer
	)

sge_sources(
	common.h
	config.h.in
	lua.h
	object.h
	physics.h
	renderer.h
	)

configure_file(
	config.h.in
	config.h
	)

sge_compile_definitions(LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/lua.h\")
