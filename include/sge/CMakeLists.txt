
sge_subdirectories(
	editor
	launcher
	renderer
	resource
	server
	)

sge_sources(
	client.hpp
	common.hpp
	compiler.hpp
	config.hpp.in
	editor.hpp
	launcher.hpp
	renderer.hpp
	resource.hpp
	scene.hpp
	server.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

