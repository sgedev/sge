
sge_sources(
	common.hpp
	config.hpp.in
	context.hpp
	light.hpp
	material.hpp
	mesh.hpp
	view.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)
