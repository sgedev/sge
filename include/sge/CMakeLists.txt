
sge_subdirectories(
	db
	gl
	gui
	scene
	sound
	)

sge_sources(
	common.hpp
	config.hpp.in
	gl.hpp
	gui.hpp
	scene.hpp
	sound.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

