
sge_sources(
	common.hpp
	config.hpp.in
	editor.hpp
	entity.hpp
	game.h
	game.hpp
	hud.hpp
	input.hpp
	player.hpp
	renderer.hpp
	scene.hpp
	signal.hpp
	view.hpp
	window.hpp
	)

sge_compile_definitions(
	LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/game.h\"
	)

configure_file(
	config.hpp.in
	config.hpp
	)

