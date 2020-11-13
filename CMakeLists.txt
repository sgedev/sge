#
#
#

cmake_minimum_required(VERSION 3.13.2)

project(sge VERSION 0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CONAN_CMAKE_URL "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake" CACHE STRING "url for 'conan.cmake' downloading.")

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from ${CONAN_CMAKE_URL}")
    file(DOWNLOAD "${CONAN_CMAKE_URL}"
        "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

add_subdirectory(engine engine)
add_subdirectory(framework framework)
add_subdirectory(test test)
add_subdirectory(samples samples)
add_subdirectory(documents documents)
