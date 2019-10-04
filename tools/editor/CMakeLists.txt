
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)

link_libraries(
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	)

add_executable(sge-editor
	consoleview.cpp
	consoleview.hpp
	main.cpp
	mainwindow.cpp
	mainwindow.hpp
	project.cpp
	project.hpp
	projectview.cpp
	projectview.hpp
	sceneview.cpp
	sceneview.hpp
	)

add_dependencies(sge-editor sge)
target_link_libraries(sge-editor sge)

