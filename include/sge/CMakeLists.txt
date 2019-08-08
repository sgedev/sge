
sge_sources(
	common.hpp
	config.hpp.in
	editor.hpp
	entity.hpp
	game.hpp
	hud.hpp
	input.hpp
	renderer.hpp
	runtime.hpp
	scene.hpp
	signal.hpp
	view.hpp
	window.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

