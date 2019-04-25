
sge_subdirectories(
	db
	game
	gl
	gui
	scene
	sound
	)

sge_sources(
	common.hpp
	config.hpp.in
	console.hpp
	db.hpp
	game.hpp
	gl.hpp
	gui.hpp
	loading.hpp
	noncopyable.hpp
	scene.hpp
	signal.hpp
	sound.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

