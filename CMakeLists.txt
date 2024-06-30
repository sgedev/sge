#
#
#

cmake_minimum_required(VERSION 3.15)

project(sge VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(SGE_DEBUG ON CACHE BOOL "Enable debug funtions.")

if(MSVC AND NOT SGE_DEBUG)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-D_HAS_CHAR8_T_LANGUAGE_SUPPORT=0)
	add_compile_options("/utf-8 /Zc:wchar_t-")
endif()

file(GLOB_RECURSE SGE_SOURCES LIST_DIRECTORIES false
	${CMAKE_SOURCE_DIR}/include/*.h
	${CMAKE_SOURCE_DIR}/include/*.hpp
	${CMAKE_SOURCE_DIR}/include/*.in
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp
)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SGE_SOURCES})

configure_file(
	include/cx/config.h.in
	include/cx/config.h
)

configure_file(
	include/GL/glex_config.h.in
	include/GL/glex_config.h
)

configure_file(
	include/sge/config.h.in
	include/sge/config.h
)

add_executable(sge ${SGE_SOURCES})

target_include_directories(sge PRIVATE
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_BINARY_DIR}/include
)

target_compile_definitions(sge PRIVATE
	LUA_USER_H=\"${CMAKE_SOURCE_DIR}/include/sge/hooks.h\"
)

include(FindOpenGL)
target_link_libraries(sge OpenGL::GL)

find_package(libuv REQUIRED)
target_link_libraries(sge uv)

find_package(PhysFS REQUIRED)
target_link_libraries(sge physfs-static)

find_package(SDL2 REQUIRED)
target_link_libraries(sge SDL2::SDL2main SDL2::SDL2-static)
