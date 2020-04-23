
include(FindOpenGL)
if(OPENGL_FOUND)
	sge_include_directories(${OPENGL_INCLUDE_DIR})
	sge_link_libraries(${OPENGL_LIBRARIES})
else()
	message(FATAL_ERROR "OpenGL not found.")
endif()

sge_compile_definitions(GLEW_STATIC)

sge_sources(
	glew.c
	glex.c
	)
