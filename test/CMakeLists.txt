
add_executable(sge-test
	Main.cpp
	)

add_dependencies(sge-test sge)
target_link_libraries(sge-test sge)
