
sge_subdirectories(
	editor
	launcher
	server
	)

sge_sources(
	camera.hpp
	common.hpp
	config.hpp.in
	editor.hpp
	entity.hpp
	game.hpp
	launcher.hpp
	level.hpp
	node.hpp
	physics.hpp
	renderer.hpp
	scene.hpp
	server.hpp
	view.hpp
	world.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

