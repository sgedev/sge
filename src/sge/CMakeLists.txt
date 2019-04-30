
include(FindUV)
if(UV_FOUND)
	sge_include_directories(${UV_INCLUDE_DIR})
	sge_link_libraries(${UV_LIBRARIES})
else()
	message(FATAL_ERROR "libuv not found.")
endif()

include(FindSDL2)
if(SDL2_FOUND)
	sge_include_directories(${SDL2_INCLUDE_DIR})
	sge_link_libraries(${SDL2_LIBRARY})
else()
	message(FATAL_ERROR "SDL2 not found.")
endif()

sge_subdirectories(
	db
	game
	gl
	scene
	renderer
	sound
	)

sge_sources(
	console.cpp
	input.cpp
	loading.cpp
	main.cpp
	)

