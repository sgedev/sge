
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ByteOrder
include(TestBigEndian)
TEST_BIG_ENDIAN(VOLCANO_BIG_ENDIAN)

# libuv
include(FindUV)
if(UV_FOUND)
    include_directories(${UV_INCLUDE_DIRS})
    link_libraries(${UV_LIBRARIES})
else()
    message(FATAL_ERROR "libuv not found.")
endif()

# PhysFS
include(FindPhysFS)
if(PHYSFS_FOUND)
    include_directories(${PHYSFS_INCLUDE_DIR})
    link_libraries(${PHYSFS_LIBRARY})
else()
    message(FATAL_ERROR "PhysFS not found.")
endif()

# SDL2
include(FindSDL2)
if(SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIR})
    link_libraries(${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY})
else()
    message(FATAL_ERROR "SDL2 not found.")
endif()

# OpenGL
include(FindOpenGL)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
    link_libraries(${OPENGL_LIBRARIES})
else()
    message(FATAL_ERROR "OpenGL not found.")
endif()

# BulletPhysics
include(FindBullet)
if(BULLET_FOUND)
    include_directories(${BULLET_INCLUDE_DIRS})
    link_libraries(${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "Bullet physics not found.")
endif()

add_definitions(-DGLEW_STATIC)

file(GLOB_RECURSE SGE_SOUSRCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

add_executable(sge ${SGE_SOUSRCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SGE_SOUSRCES})
configure_file(include/sge/config.h.in include/sge/config.h)