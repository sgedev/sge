
sge_subdirectories(
	db
	game
	renderer
	scene
	sound
	)

sge_sources(
	common.hpp
	config.hpp.in
	console.hpp
	db.hpp
	game.hpp
	input.hpp
	loading.hpp
	noncopyable.hpp
	renderer.hpp
	scene.hpp
	signal.hpp
	sound.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

