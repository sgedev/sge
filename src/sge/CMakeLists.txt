
include(FindSDL2)
if(SDL2_FOUND)
	target_include_directories(sge PRIVATE ${SDL2_INCLUDE_DIR})
	target_link_libraries(sge ${SDL2_LIBRARY})
else()
	message(FATAL_ERROR "SDL2 not found.")
endif()

target_link_libraries(sge ${CMAKE_DL_LIBS})

if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    target_link_libraries(sge stdc++fs)
endif()

target_sources(sge
	PRIVATE
		list.c
	)

add_subdirectory(boot boot)
add_subdirectory(graphics graphics)
add_subdirectory(gui gui)
add_subdirectory(scene scene)
add_subdirectory(sound sound)
add_subdirectory(vm vm)
