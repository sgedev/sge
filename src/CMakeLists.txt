
include(FindSDL2)
if(SDL2_FOUND)
	target_include_directories(sge PRIVATE ${SDL2_INCLUDE_DIR})
	target_link_libraries(sge ${SDL2_LIBRARY})
else()
	message(FATAL_ERROR "SDL2 not found.")
endif()

include(FindBullet)


add_subdirectory(gl3w gl3w)
add_subdirectory(lua lua)
add_subdirectory(miniz miniz)
add_subdirectory(nanovg nanovg)
add_subdirectory(rttr rttr)
add_subdirectory(sge sge)
add_subdirectory(uv uv)
