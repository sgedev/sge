
include(FindOpenGL)
if(OPENGL_FOUND)
	target_include_directories(sge PRIVATE ${OPENGL_INCLUDE_DIR})
	target_link_libraries(sge ${OPENGL_LIBRARIES})
else()
	message(FATAL_ERROR "OpenGL not found.")
endif()

target_sources(sge
	PRIVATE
		glew.c
	)
