
file(GLOB DENTRY_LIST LIST_DIRECTORIES true RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")
foreach(DENTRY ${DENTRY_LIST})
	if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DENTRY}")
		list(APPEND SGE_GAME_LIST ${DENTRY})
	endif()
endforeach()

if(NOT SGE_GAME_LIST)
	message(FATAL_ERROR "No game found.")
endif()

list(GET SGE_GAME_LIST 0 SGE_GAME_DEFAULT)
set(SGE_GAME ${SGE_GAME_DEFAULT} CACHE STRING "Select game to build.")
set_property(CACHE SGE_GAME PROPERTY STRINGS ${SGE_GAME_LIST})
if(NOT SGE_GAME)
	message(FATAL_ERROR "No game specified.")
endif()

sge_subdirectories(${SGE_GAME})
sge_properties(OUTPUT_NAME ${SGE_GAME})
sge_compile_definitions(SGE_GAME_NAME=\"${SGE_GAME}\")

