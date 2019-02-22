#
#
#

target_include_directories(sge PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)

add_subdirectory(sge sge)

