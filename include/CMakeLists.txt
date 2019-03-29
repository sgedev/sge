#
#
#

target_include_directories(sge PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)

add_subdirectory(GL GL)
add_subdirectory(sge sge)

target_sources(sge
	PRIVATE
		miniz.h
		miniz_common.h
		miniz_tdef.h
		miniz_tinfl.h
		miniz_zip.h
		pugiconfig.hpp
		pugixml.hpp
	)

