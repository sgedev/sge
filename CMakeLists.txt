#
#
#

cmake_minimum_required(VERSION 3.13.2)

project(sge VERSION 0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
endif()

# sge macros

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

# sge

add_executable(sge)

set(SGE_DEBUG ON CACHE BOOL "Enable debug functions.")
set(SGE_LOG ON CACHE BOOL "Enable log functions.")

sge_include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_BINARY_DIR}/include
	)

sge_subdirectories(
	include
	src
	)

set_property(TARGET sge PROPERTY C_STANDARD 99)
get_target_property(SGE_SOURCES sge SOURCES)
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SGE_SOURCES})

add_subdirectory(docs docs)
add_subdirectory(test test)
add_subdirectory(tools tools)
