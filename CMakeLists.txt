#
#
#

cmake_minimum_required(VERSION 3.19)

project(sge VERSION 0.1.0)

if(MSVC)
    add_definitions(-D _CRT_SECURE_NO_WARNINGS)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake")

set(SGE_DEBUG ON CACHE BOOL "Enable debug functions.")

add_subdirectory(documents document)
add_subdirectory(sources sources)
