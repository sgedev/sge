
target_sources(sge
	PRIVATE
		common.hpp
		config.hpp.in
		physics.hpp
		renderer.hpp
		sound.hpp
		vm.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

add_subdirectory(physics physics)
add_subdirectory(renderer renderer)
add_subdirectory(sound sound)
add_subdirectory(vm vm)
