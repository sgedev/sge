
add_executable(sge-server
	main.cpp
	)

add_dependencies(sge-server sge)
target_link_libraries(sge-server sge)

