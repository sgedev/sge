
add_executable(sge-editor
	main.cpp
	)

add_dependencies(sge-editor sge)
target_link_libraries(sge-editor sge)

