
target_sources(sge
	PRIVATE
		common.hpp
		config.hpp.in
		renderer.hpp
		sound.hpp
		vm.hpp
	)

configure_file(
	config.hpp.in
	config.hpp
	)

add_subdirectory(renderer renderer)
add_subdirectory(sound sound)
add_subdirectory(vm vm)
