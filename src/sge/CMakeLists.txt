
target_sources(sge
	PRIVATE
		core.cpp
	)

add_subdirectory(physics physics)
add_subdirectory(renderer renderer)
add_subdirectory(sound sound)
add_subdirectory(vm vm)
