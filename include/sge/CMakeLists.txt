
sge_subdirectories(
	renderer
	)

sge_sources(
	common.hpp
	config.hpp.in
	game.h
	game.hpp
	renderer.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

sge_compile_definitions(
	LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/game.h\"
	)

