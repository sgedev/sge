
include(FindSDL2)
if(SDL2_FOUND)
	sge_include_directories(${SDL2_INCLUDE_DIR})
	sge_link_libraries(${SDL2_LIBRARY})
else()
	message(FATAL_ERROR "SDL2 not found.")
endif()

sge_subdirectories(
	GL
	imgui
	SGL
	)

sge_sources(
	editor.cpp
	editor.hpp
	main.cpp
	)
