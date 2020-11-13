#
#
#

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(engine)

target_include_directories(engine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

file(GLOB_RECURSE SGE_INCLUDES "*.h" "*.hpp")
file(GLOB_RECURSE SGE_SOURCES "*.c" "*.cpp")
file(GLOB_RECURSE SGE_CONFIGS "*.in")

set(SGE_ALL_SOURCES ${SGE_INCLUDES} ${SGE_SOURCES} ${SGE_CONFIGS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SGE_ALL_SOURCES})
target_sources(engine PRIVATE ${SGE_ALL_SOURCES})
set_target_properties(engine PROPERTIES OUTPUT_NAME "sge")

target_compile_definitions(engine PRIVATE LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/include/sge/vm/task.h\")

set(SGE_DEBUG ON CACHE BOOL "Enable debug functions.")

foreach(CONFIG_PATH ${SGE_CONFIGS})
    file(RELATIVE_PATH CONFIG_RPATH ${CMAKE_CURRENT_SOURCE_DIR} ${CONFIG_PATH})
    string(REGEX REPLACE "\\.in\$" "" CONFIG_OUTPUT_RPATH ${CONFIG_RPATH})
    configure_file(${CONFIG_RPATH} ${CONFIG_OUTPUT_RPATH})
endforeach()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    target_link_libraries(engine stdc++fs)
endif()

conan_cmake_run(
    REQUIRES
        assimp/5.0.1
        bullet3/2.89
        libuv/1.40.0
        sdl2/2.0.12@bincrafters/stable
    BASIC_SETUP
    CMAKE_TARGETS
    BUILD_TYPE "Release"
    BUILD missing
    )

target_link_libraries(engine
    CONAN_PKG::assimp
    CONAN_PKG::bullet3
    CONAN_PKG::libuv
    CONAN_PKG::sdl2
    )
