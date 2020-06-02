#
#
#

cmake_minimum_required(VERSION 3.13.2)

project(SGE VERSION 0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
endif()

set(CMAKE_AUTOMOC ON)

find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Concurrent REQUIRED)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_BINARY_DIR}/include
	)

set(SGE_DEBUG ON CACHE BOOL "Enable debug functions.")
set(SGE_LOG ON CACHE BOOL "Enable log functions.")

add_library(sge SHARED)
target_link_libraries(sge Qt5::Gui Qt5::Qml Qt5::Concurrent)

add_subdirectory(include include)
add_subdirectory(src src)

get_target_property(SGE_SOURCES sge SOURCES)
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SGE_SOURCES})

add_subdirectory(docs docs)
add_subdirectory(test test)
add_subdirectory(tools tools)
