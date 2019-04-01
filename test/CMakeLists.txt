#
#
#

include(FindSDL2)
if(SDL2_FOUND)
	include_directories(${SDL2_INCLUDE_DIR})
	link_libraries(${SDL2_LIBRARY})
else()
	message(FATAL_ERROR "SDL2 not found.")
endif()

link_libraries(sge)

add_executable(sge-test main.cpp)
add_dependencies(sge-test sge)

