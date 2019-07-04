
sge_subdirectories(
	game
	gui
	scene
	sound
	)

sge_sources(
	common.hpp
	config.hpp.in
	game.hpp
	gui.hpp
	noncopyable.hpp
	scene.hpp
	signal.hpp
	sound.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

