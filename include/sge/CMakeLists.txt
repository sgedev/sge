
sge_sources(
	camera.hpp
	common.hpp
	config.hpp.in
	editor.hpp
	entity.hpp
	game.hpp
	gl.hpp
	input.hpp
	noncopyable.hpp
	player.hpp
	renderer.hpp
	scene.hpp
	server.hpp
	signal.hpp
	subsystem.hpp
	window.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

