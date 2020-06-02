
find_package(Qt5Widgets REQUIRED)

add_sge_tool(sge-editor
	Main.cpp
	MainWindow.cpp
	MainWindow.hpp
	)

target_link_libraries(sge-editor Qt5::Widgets)
