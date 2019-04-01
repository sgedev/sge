#
#
#

cmake_minimum_required(VERSION 3.13.2)

project(sge VERSION 0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_C_STANDARD 99)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
endif()

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_BINARY_DIR}/include
	)

set(SGE_DEBUG ON CACHE BOOL "Enable debug features.")
set(SGE_SHARED ON CACHE BOOL "Build shared library.")

if(SGE_SHARED)
	add_library(sge SHARED)
	target_compile_definitions(sge PRIVATE SGE)
else()
	add_library(sge STATIC)
endif()

add_subdirectory(include include)
add_subdirectory(src src)

get_target_property(SGE_SOURCES sge SOURCES)
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SGE_SOURCES})

install(
	TARGETS
		sge
	RUNTIME
		DESTINATION bin
		COMPONENT Runtime
	LIBRARY
		DESTINATION lib
		COMPONENT Library
	ARCHIVE
		DESTINATION lib
		COMPONENT Library
	PUBLIC_HEADER
		DESTINATION include
		COMPONENT Development
	)

# others
add_subdirectory(docs docs)
add_subdirectory(test test)
add_subdirectory(tools tools)

# package
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "sgedev@gmail.com")
include(CPack)

