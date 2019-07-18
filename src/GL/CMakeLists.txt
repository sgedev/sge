
include(FindOpenGL)
if(OPENGL_FOUND)
	sge_include_directories(${OPENGL_INCLUDE_DIR})
	sge_link_libraries(${OPENGL_LIBRARIES})
	sge_compile_definitions(GLEW_STATIC)
else()
	message(FATAL_ERROR "OpenGL not found.")
endif()

sge_sources(
	buffer.cpp
	glew.c
	program.cpp
	shader.cpp
	vertexarray.cpp
	)

