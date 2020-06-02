
macro(add_sge_tool TARGET)
	add_executable(${TARGET} ${ARGN})
	add_dependencies(${TARGET} sge)
	target_link_libraries(${TARGET} sge)
	set_target_properties(${TARGET} PROPERTIES FOLDER "tools")
endmacro()

add_subdirectory(Editor Editor)
add_subdirectory(Launcher Launcher)
add_subdirectory(Server Server)
