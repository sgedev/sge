
sge_sources(
	camera.hpp
	common.hpp
	config.hpp.in
	editor.hpp
	entity.hpp
	hud.hpp
	input.hpp
	player.hpp
	renderer.hpp
	runtime.hpp
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

