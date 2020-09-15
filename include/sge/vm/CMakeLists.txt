
target_sources(sge
	PRIVATE
		common.hpp
		gui.hpp
		output.hpp
		resource.hpp
		scene.hpp
		task.h
	)

target_compile_definitions(sge PRIVATE LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/task.h\")

add_subdirectory(gui gui)
add_subdirectory(scene scene)
