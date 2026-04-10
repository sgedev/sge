
cmake_minimum_required(VERSION 3.24)
project(sge VERSION 0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

set(SGE_DEBUG ON CACHE BOOL "Enable debug functions.")
set(SGE_PROFILE ON CACHE BOOL "Enable profile functions.")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(LUA_HEADERS
	include/lauxlib.h
	include/lua.h
	include/lua.hpp
	include/luaconf.h
	include/lualib.h
	src/lua/lapi.h
	src/lua/lcode.h
	src/lua/lctype.h
	src/lua/ldebug.h
	src/lua/ldo.h
	src/lua/lfunc.h
	src/lua/lgc.h
	src/lua/ljumptab.h
	src/lua/llex.h
	src/lua/llimits.h
	src/lua/lmem.h
	src/lua/lobject.h
	src/lua/lopcodes.h
	src/lua/lopnames.h
	src/lua/lparser.h
	src/lua/lprefix.h
	src/lua/lstate.h
	src/lua/lstring.h
	src/lua/ltable.h
	src/lua/ltm.h
	src/lua/lundump.h
	src/lua/lvm.h
	src/lua/lzio.h
)

set(LUA_SOURCES
	src/lua/lapi.c
	src/lua/lauxlib.c
	src/lua/lbaselib.c
	src/lua/lcode.c
	src/lua/lcorolib.c
	src/lua/lctype.c
	src/lua/ldblib.c
	src/lua/ldebug.c
	src/lua/ldo.c
	src/lua/ldump.c
	src/lua/lfunc.c
	src/lua/lgc.c
	src/lua/linit.c
	src/lua/liolib.c
	src/lua/llex.c
	src/lua/lmathlib.c
	src/lua/lmem.c
	src/lua/loadlib.c
	src/lua/lobject.c
	src/lua/lopcodes.c
	src/lua/loslib.c
	src/lua/lparser.c
	src/lua/lstate.c
	src/lua/lstring.c
	src/lua/lstrlib.c
	src/lua/ltable.c
	src/lua/ltablib.c
	src/lua/ltm.c
	src/lua/lundump.c
	src/lua/lutf8lib.c
	src/lua/lvm.c
	src/lua/lzio.c
)

set_source_files_properties(${LUA_SOURCES} PROPERTIES LANGUAGE CXX)

set(SGE_SOURCES
	${LUA_HEADERS}
	${LUA_SOURCES}
	include/sge/acoustic/common.hpp
	include/sge/graphics/common.hpp
	include/sge/system/base.hpp
	include/sge/system/client.hpp
	include/sge/system/common.hpp
	include/sge/system/local.hpp
	include/sge/system/server.hpp
	include/sge/system/window.hpp
	include/sge/vm/common.hpp
	include/sge/vm/context.hpp
	include/sge/vm/kernel.hpp
	include/sge/vm/hooks.h
	include/sge/common.h
	include/sge/common.hpp
	include/sge/config.h.in
	include/sge/list.h
	include/sge/scopeguard.hpp
	src/sge/vm/kernel.cpp
	src/sge/system/base.cpp
	src/sge/system/client.cpp
	src/sge/system/local.cpp
	src/sge/system/main.cpp
	src/sge/system/server.cpp
	src/sge/system/window.cpp
)

add_executable(sge ${SGE_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SGE_SOURCES})

target_compile_definitions(sge PRIVATE
	LUA_USER_H=\"sge/vm/hooks.h\"
)

target_include_directories(sge PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_BINARY_DIR}/include
)

configure_file(
	include/sge/config.h.in
	include/sge/config.h
)

find_package(argh)
target_link_libraries(sge argh)

find_package(Eigen3)
target_link_libraries(sge Eigen3::Eigen)

find_package(enet)
target_link_libraries(sge enet::enet)

find_package(EnTT)
target_link_libraries(sge EnTT::EnTT)

find_package(glad)
target_link_libraries(sge glad::glad)

find_package(Jolt)
target_link_libraries(sge Jolt::Jolt)

find_package(libuv)
target_link_libraries(sge libuv::uv_a)

find_package(PhysFS)
target_link_libraries(sge physfs-static)

find_package(SFML)
target_link_libraries(sge sfml::sfml)

find_package(spdlog)
target_link_libraries(sge spdlog::spdlog)

find_package(Tracy)
target_link_libraries(sge Tracy::TracyClient)

set(SGE_TOOLS ON CACHE BOOL "Enable tools.")
if(SGE_TOOLS)
	add_subdirectory(tools)
endif()

set(SGE_TEST OFF CACHE BOOL "Enable test.")
if(SGE_TEST)
	add_subdirectory(test)
endif()

set(SGE_DOCUMENTS ON CACHE BOOL "Enable documents.")
if(SGE_DOCUMENTS)
	add_subdirectory(docs)
endif()
