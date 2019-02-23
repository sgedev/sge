#
#
#

add_subdirectory(gl gl)
add_subdirectory(gui gui)
add_subdirectory(physics physics)
add_subdirectory(renderer renderer)

target_sources(sge
	PRIVATE
		common.h
		config.h.in
		game.h
		gl.h
		gui.h
		physics.h
		renderer.h
		sound.h
	)

configure_file(
	config.h.in
	config.h
	)

