#
#
#

cmake_minimum_required(VERSION 3.13.2)

project(sge VERSION 0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
endif()

macro(sge_subdirectories)
	foreach(SUBDIR ${ARGN})
		add_subdirectory(${SUBDIR} ${SUBDIR})
	endforeach()
endmacro()

macro(sge_sources)
	target_sources(sge PRIVATE ${ARGN})
endmacro()

macro(sge_include_directories)
	target_include_directories(sge PRIVATE ${ARGN})
endmacro()

macro(sge_compile_definitions)
	target_compile_definitions(sge PRIVATE ${ARGN})
endmacro()

macro(sge_link_libraries)
	target_link_libraries(sge ${ARGN})
endmacro()

macro(sge_properties)
	set_target_properties(sge PROPERTIES ${ARGN})
endmacro()

add_executable(sge)

include(FindUV)
if(UV_FOUND)
	sge_include_directories(${UV_INCLUDE_DIRS})
	sge_link_libraries(${UV_LIBRARIES})
else()
	message(FATAL_ERROR "libuv not found.")
endif()

include(FindSDL2)
if(SDL2_FOUND)
	sge_include_directories(${SDL2_INCLUDE_DIR})
	sge_link_libraries(${SDL2_LIBRARY})
else()
	message(FATAL_ERROR "SDL2 not found.")
endif()

include(FindGLM)
if(GLM_FOUND)
	sge_include_directories(${GLM_INCLUDE_DIR})
else()
	message(FATAL_ERRO "glm not found.")
endif()

sge_include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_BINARY_DIR}/include
	)

sge_subdirectories(
	docs
	extras
	fs
	games
	gui
	include
	init
	kernel
	lib
	renderer
	scene
	sound
	test
	tools
	)

get_target_property(SGE_SOURCES sge SOURCES)
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SGE_SOURCES})

