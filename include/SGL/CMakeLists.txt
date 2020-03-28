
sge_subdirectories(
	renderer
	)

sge_sources(
	common.hpp
	config.hpp.in
	renderer.hpp
	scene.hpp
	window.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)
