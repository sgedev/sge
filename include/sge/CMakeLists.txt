
sge_subdirectories(
	editor
	launcher
	renderer
	server
	)

sge_sources(
	client.hpp
	common.hpp
	renderer.hpp
	scene.hpp
	config.hpp.in
	)

configure_file(
	config.hpp.in
	config.hpp
	)

