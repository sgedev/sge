
sge_subdirectories(
	game
	gl
	scene
	sound
	)

sge_sources(
	common.hpp
	config.hpp.in
	game.hpp
	gl.hpp
	noncopyable.hpp
	scene.hpp
	server.hpp
	signal.hpp
	sound.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

