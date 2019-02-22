
include(FindOpenGL)
if(OpenGL_OpenGL_FOUND)
	target_include_directories(sge PRIVATE ${OPENGL_INCLUDE_DIR})
	target_link_libraries(sge ${OPENGL_LIBRARIES})
else()
	message(FATAL_ERROR "OpenGL not found.")
endif()

include(FindGLEW)
if(GLEW_FOUND)
	target_include_directories(sge PRIVATE ${GLEW_INCLUDE_DIRS})
	target_link_libraries(sge ${GLEW_LIBRARIES})
else()
	message(FATAL_ERROR "GLEW not found.")
endif()

set(SGE_LINK_STATIC_GLEW OFF CACHE BOOL "Link static glew library.")
if(SGE_LINK_STATIC_GLEW)
	target_compile_definitions(sge PRIVATE GLEW_STATIC)
endif()

target_sources(sge
	PRIVATE
		buffer.cpp
		core.cpp
		program.cpp
		shader.cpp
		vertex_array.cpp
	)

