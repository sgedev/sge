
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
	game.h
	game.hpp
	launcher.hpp
	level.hpp
	renderer.hpp
	scene.hpp
	server.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

sge_compile_definitions(
	LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/game.h\"
	)
