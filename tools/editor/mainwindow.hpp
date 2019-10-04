//
//
#ifndef SGE_EDITOR_MAINWINDOW_HPP
#define SGE_EDITOR_MAINWINDOW_HPP

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QDockWidget>

#include "consoleview.hpp"
#include "projectview.hpp"
#include "sceneview.hpp"
#include "project.hpp"

class MainWindow: public QMainWindow {
	Q_OBJECT

public:
	MainWindow(void);
	virtual ~MainWindow(void);

public:
	bool Init(void);

private:
	QMdiArea m_mdiArea;

	QMdiSubWindow m_sceneViewSubWindow;
	SceneView m_sceneView;

	QDockWidget m_projectViewDocker;
	ProjectView m_projectView;

	QDockWidget m_consoleViewDocker;
	ConsoleView m_consoleView;

	Project m_project;
};

#endif // SGE_EDITOR_MAINWINDOW_HPP

