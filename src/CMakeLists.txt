
include(FindUV)
if(UV_FOUND)
	target_include_directories(sge PRIVATE ${UV_INCLUDE_DIRS})
	target_link_libraries(sge ${UV_LIBRARIES})
else()
	message(FATAL_ERROR "libuv not found.")
endif()

include(FindSDL2)
if(SDL2_FOUND)
	target_include_directories(sge PRIVATE ${SDL2_INCLUDE_DIR})
	target_link_libraries(sge ${SDL2_LIBRARY})
else()
	message(FATAL_ERROR "SDL2 not found.")
endif()

target_link_libraries(sge ${CMAKE_DL_LIBS})

target_sources(sge
	PRIVATE
		main.cpp
	)

add_subdirectory(cx cx)
add_subdirectory(glew glew)
add_subdirectory(lua lua)
add_subdirectory(nanovg nanovg)
add_subdirectory(physfs physfs)
add_subdirectory(rttr rttr)
add_subdirectory(sge sge)
