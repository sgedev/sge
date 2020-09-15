
target_sources(sge
	PRIVATE
		core.cpp
		output.cpp
		resource.cpp
	)

add_subdirectory(gui gui)
add_subdirectory(scene scene)
