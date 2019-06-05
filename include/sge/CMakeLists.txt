
sge_subdirectories(
	editor
	game
	renderer
	scene
	sound
	)

sge_sources(
	common.hpp
	config.hpp.in
	console.hpp
	editor.hpp
	game.hpp
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

