#
#
#

# dep: Boost
set(BOOST_INCLUDEDIR "" CACHE PATH "Path to boost include directory.")
set(BOOST_LIBRARYDIR "" CACHE PATH "Path to boost library directory.")
set(Boost_USE_STATIC_LIBS ON)
include(FindBoost)
find_package(Boost 1.36.0)
if(Boost_FOUND)
	target_include_directories(sge PRIVATE ${Boost_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "Boost not found.")
endif()

# dep: Eigen3
include(FindEigen3)
if(EIGEN3_FOUND)
	target_include_directories(sge PRIVATE ${EIGEN3_INCLUDE_DIR})
else()
	message(FATAL_ERROR "Eigen3 not found.")
endif()

# dep: SDL2
include(FindSDL2)
if(SDL2_FOUND)
	target_include_directories(sge PRIVATE ${SDL2_INCLUDE_DIR})
	target_link_libraries(sge ${SDL2_LIBRARY})
else()
	message(FATAL_ERROR "SDL2 not found.")
endif()

# dep: Zlib
#include(FindZLIB)
#if(ZLIB_FOUND)
#	target_include_directories(sge PRIVATE ${ZLIB_INCLUDE_DIRS})
#	target_link_libraries(sge ${ZLIB_LIBRARIES})
#else()
#	message(FATAL_ERROR "Zlib not found.")
#endif()

# dep: Assimp
#include(FindAssimp)
#if(ASSIMP_FOUND)
#	target_include_directories(${ASSIMP_INCLUDE_DIR})
#	target_link_libraries(sge ${ASSIMP_LIBRARY})
#else()
#	message(FATAL_ERROR "assimp not found.")
#endif()

# sge

target_sources(sge
	PRIVATE
		main.cpp
		sound.cpp
	)

add_subdirectory(fs fs)
add_subdirectory(game game)
add_subdirectory(gl gl)
add_subdirectory(gui gui)
add_subdirectory(physics physics)
add_subdirectory(renderer renderer)
