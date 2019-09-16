
sge_sources(
	common.hpp
	config.hpp.in
	editor.hpp
	entity.hpp
	game.h
	game.hpp
	player.hpp
	scene.hpp
	server.hpp
	subsystem.hpp
	)

sge_compile_definitions(
	LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/game.h\"
	)

configure_file(
	config.hpp.in
	config.hpp
	)

