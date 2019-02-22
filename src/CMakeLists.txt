#
#
#

# dep: Eigen3
include(FindEigen3)
if(EIGEN3_FOUND)
	target_include_directories(sge PRIVATE ${EIGEN3_INCLUDE_DIR})
else()
	message(FATAL_ERROR "Eigen3 not found.")
endif()

# dep: SDL2
include(FindSDL2)
if(SDL2_FOUND)
	target_include_directories(sge PRIVATE ${SDL2_INCLUDE_DIR})
	target_link_libraries(sge ${SDL2_LIBRARY})
else()
	message(FATAL_ERROR "SDL2 not found.")
endif()

# sge

target_sources(sge
	PRIVATE
		main.cpp
		game.cpp
		sound.cpp
	)

add_subdirectory(game game)
add_subdirectory(gl gl)
add_subdirectory(gui gui)
add_subdirectory(microprofile microprofile)
add_subdirectory(physics physics)
add_subdirectory(renderer renderer)
add_subdirectory(scene scene)

