
include(FindSDL2)
if(SDL2_FOUND)
	include_directories(${SDL2_INCLUDE_DIR})
	link_libraries(${SDL2_LIBRARY})
else()
	message(FATAL_ERROR "SDL2 not found.")
endif()

include(FindPhysFS)
if(PHYSFS_FOUND)
	include_directories(${PHYSFS_INCLUDE_DIR})
	link_libraries(${PHYSFS_LIBRARY})
else()
	message(FATAL_ERROR "PhysFS not found.")
endif()

add_executable(sge-launcher
	filesystem.cpp
	filesystem.hpp
	game.cpp
	game.hpp
	main.cpp
	)

add_dependencies(sge-launcher sge)
target_link_libraries(sge-launcher sge)

