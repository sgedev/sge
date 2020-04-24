
include(FindSDL2)
if(SDL2_FOUND)
	sge_include_directories(${SDL2_INCLUDE_DIR})
	sge_link_libraries(${SDL2_LIBRARY})
else()
	message(FATAL_ERROR "SDL2 not found.")
endif()

include(FindUV)
if(UV_FOUND)
	sge_include_directories(${UV_INCLUDE_DIRS})
	sge_link_libraries(${UV_LIBRARIES})
else()
	message(FATAL_ERROR "libuv not found.")
endif()

sge_subdirectories(
	gui
	scene
	)

sge_sources(
	camera.c
	common.c
	input.c
	main.c
	vm.c
	window.c
	)

