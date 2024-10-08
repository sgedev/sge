
cmake_minimum_required(VERSION 3.24)
project(sge)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

set(SGE_DEBUG ON CACHE BOOL "Enable debug functions.")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(sge/Utils)

add_subdirectory(runtime)
add_subdirectory(framework)
add_subdirectory(tools)
add_subdirectory(system)
add_subdirectory(documents)
