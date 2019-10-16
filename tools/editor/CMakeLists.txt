
include(FindAssimp)
if(ASSIMP_FOUND)
	include_directories(${ASSIMP_INCLUDE_DIR})
	link_libraries(${ASSIMP_LIBRARY})
else()
	message(FATAL_ERROR "Assimp not found.")
endif()

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Xml)

link_libraries(
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::Xml
	)

add_executable(sge-editor
	common.hpp
	consoleview.cpp
	consoleview.hpp
	game.cpp
	game.hpp
	importer.cpp
	importer.hpp
	main.cpp
	mainwindow.cpp
	mainwindow.hpp
	newfolderdialog.cpp
	newfolderdialog.hpp
	newprojectdialog.cpp
	newprojectdialog.hpp
	newscenedialog.cpp
	newscenedialog.hpp
	newscriptdialog.cpp
	newscriptdialog.hpp
	project.cpp
	project.hpp
	projectview.cpp
	projectview.hpp
	sceneview.cpp
	sceneview.hpp
	)

add_dependencies(sge-editor sge)
target_link_libraries(sge-editor sge)

