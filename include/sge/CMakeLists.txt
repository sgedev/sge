
sge_compile_definitions(
	LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/game.h\"
	)

sge_sources(
	common.hpp
	config.hpp.in
	game.h
	game.hpp
	object.hpp
	renderer.hpp
	scene.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)
