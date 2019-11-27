
sge_subdirectories(
	database
	editor
	launcher
	renderer
	server
	)

sge_sources(
	asset.hpp
	common.hpp
	config.hpp.in
	database.hpp
	editor.hpp
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

