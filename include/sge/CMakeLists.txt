#
#
#

add_subdirectory(gl gl)
add_subdirectory(gui gui)

target_sources(sge
	PRIVATE
		camera.hpp
		common.hpp
		config.hpp.in
		fs.hpp
		gl.hpp
		gui.hpp
		scene.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

