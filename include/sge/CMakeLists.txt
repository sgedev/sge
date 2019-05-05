
sge_subdirectories(
	db
	fs
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
	fs.hpp
	game.hpp
	gl.hpp
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

