
sge_subdirectories(
	editor
	launcher
	renderer
	server
	)

sge_sources(
	client.hpp
	common.hpp
	config.hpp.in
	editor.hpp
	filesystem.hpp
	launcher.hpp
	renderer.hpp
	scene.hpp
	server.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

