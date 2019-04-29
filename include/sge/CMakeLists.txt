
sge_subdirectories(
	db
	game
	gl
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

