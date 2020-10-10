
target_sources(sge
	PRIVATE
		color.hpp
		common.h
		common.hpp
		config.h.in
		list.h
	)

configure_file(
	config.h.in
	config.h
	)

add_subdirectory(boot boot)
add_subdirectory(graphics graphics)
add_subdirectory(gui gui)
add_subdirectory(scene scene)
add_subdirectory(sound sound)
add_subdirectory(vm vm)
