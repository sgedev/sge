
include(FindOpenGL)
if(OpenGL_OpenGL_FOUND)
	target_include_directories(sge PRIVATE ${OPENGL_INCLUDE_DIR})
	target_link_libraries(sge ${OPENGL_LIBRARIES})
else()
	message(FATAL_ERROR "OpenGL not found.")
endif()

target_sources(sge
	PRIVATE
		eglew.h
		glew.c
		glew.h
		glxew.h
		wglew.h
	)

target_compile_definitions(sge
	PRIVATE
		GLEW_STATIC
	)

