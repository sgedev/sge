
set(SGE_EDITOR ON CACHE BOOL "Build level editor.")
if(NOT SGE_EDITOR)
	return()
endif()

include(FindAssimp)
if(ASSIMP_FOUND)
	sge_include_directories(${ASSIMP_INCLUDE_DIR})
	sge_link_libraries(${ASSIMP_LIBRARY})
else()
	message(FATAL_ERROR "Assimp not found.")
endif()

sge_sources(
	application.cpp
	assetview.cpp
	consoleview.cpp
	game.cpp
	importer.cpp
	mainwindow.cpp
	newfolderdialog.cpp
	newprojectdialog.cpp
	newscenedialog.cpp
	newscriptdialog.cpp
	project.cpp
	projectview.cpp
	sceneview.cpp
	)

