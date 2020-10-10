
target_sources(sge
	PRIVATE
		common.hpp
		kernel.hpp
		task.h
	)

target_compile_definitions(sge PRIVATE LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/task.h\")
