
sge_subdirectories(
	editor
	launcher
	renderer
	server
	)

sge_sources(
	common.hpp
	config.hpp.in
	editor.hpp
	game.hpp
	launcher.hpp
	level.hpp
	lua.h
	renderer.hpp
	scene.hpp
	server.hpp
	vm.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

sge_compile_definitions(
	LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/lua.h\"
	)
