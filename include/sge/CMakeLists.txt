#
#
#

add_subdirectory(gl gl)
add_subdirectory(gui gui)
add_subdirectory(physics physics)
add_subdirectory(renderer renderer)
add_subdirectory(scene scene)

target_sources(sge
	PRIVATE
		common.hpp
		config.hpp.in
		game.hpp
		gl.hpp
		gui.hpp
		physics.hpp
		renderer.hpp
		scene.hpp
		sound.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

