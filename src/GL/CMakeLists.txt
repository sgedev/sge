
include(FindOpenGL)
if(OPENGL_FOUND)
	sge_include_directories(${OPENGL_INCLUDE_DIR})
	sge_link_libraries(${OPENGL_LIBRARIES})
	sge_compile_definitions(GLEW_STATIC)
else()
	message(FATAL_ERROR "OpenGL not found.")
endif()

sge_sources(
	glew.c
	glex_common.h
	glex_core.c
	glex_light.c
	glex_light.h
	glex_material.c
	glex_material.h
	glex_mesh.c
	glex_mesh.h
	glex_program.c
	glex_program.h
	glex_test.c
	glex_test.h
	)
