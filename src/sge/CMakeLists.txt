
# bullet libuv

include(FindUV)
if(UV_FOUND)
	sge_include_directories(${UV_INCLUDE_DIRS})
	sge_link_libraries(${UV_LIBRARIES})
else()
	message(FATAL_ERROR "Libuv not foun.d")
endif()

sge_subdirectories(
	game
	scene
	renderer
	)

sge_sources(
	core.cpp
	object.cpp
	)
