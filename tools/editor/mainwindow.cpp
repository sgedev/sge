//
//
#include <QList>
#include <QSettings>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QIcon>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>

#include <iostream>
#include <physfs.h>

#include "common.hpp"
#include "newprojectdialog.hpp"
#include "mainwindow.hpp"

MainWindow::MainWindow(void)
{
}

MainWindow::~MainWindow(void)
{
}

bool MainWindow::Init(void)
{
	QSettings settings;
	restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
	restoreState(settings.value("MainWindow/state").toByteArray());

	restoreDockWidget(&m_projectViewDocker);
	restoreDockWidget(&m_consoleViewDocker);

	m_fileMenu = menuBar()->addMenu("&File");

	m_fileNewProject = m_fileMenu->addAction("New...");
	connect(m_fileNewProject, &QAction::triggered, this, &MainWindow::newProject);

	m_fileOpenProject = m_fileMenu->addAction("Open...");
	connect(m_fileOpenProject, &QAction::triggered, this, &MainWindow::openProject);

	m_fileCloseProject = m_fileMenu->addAction("Close");
	m_fileMenu->addSeparator();
	m_fileImport = m_fileMenu->addAction("Import...");
	m_fileExport = m_fileMenu->addAction("Export...");
	m_fileMenu->addSeparator();
	m_fileQuit = m_fileMenu->addAction("Quit");

	m_editMenu = menuBar()->addMenu("&Edit");
	m_editCut = m_editMenu->addAction("Cut");
	m_editCopy = m_editMenu->addAction("&Copy");
	m_editPaste = m_editMenu->addAction("Paste");

	m_viewMenu = menuBar()->addMenu("&View");
	m_viewProject = m_projectViewDocker.toggleViewAction();
	m_viewProject->setText("Project");
	m_viewMenu->addAction(m_viewProject);
	m_viewConsole = m_consoleViewDocker.toggleViewAction();
	m_viewConsole->setText("Console");
	m_viewMenu->addAction(m_viewConsole);

	m_toolMenu = menuBar()->addMenu("&Tool");
	m_toolOptions = m_toolMenu->addAction("&Options");

	m_helpMenu = menuBar()->addMenu("&Help");
	m_helpAbout = m_helpMenu->addAction("&About...");

	setCentralWidget(&m_mdiArea);

	m_mdiArea.setViewMode(QMdiArea::TabbedView);
	m_mdiArea.setTabsClosable(true);
	m_mdiArea.setTabsMovable(true);

	m_sceneViewSubWindow.setWidget(&m_sceneView);
	m_mdiArea.addSubWindow(&m_sceneViewSubWindow);
	m_sceneViewSubWindow.showMaximized();

	m_projectViewDocker.setWidget(&m_projectView);
	addDockWidget(Qt::LeftDockWidgetArea, &m_projectViewDocker);

	m_consoleViewDocker.setWidget(&m_consoleView);
	addDockWidget(Qt::BottomDockWidgetArea, &m_consoleViewDocker);

	statusBar()->showMessage("Ready");

	return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QSettings settings;

	settings.setValue("MainWindow/geometry", saveGeometry());
	settings.setValue("MainWindow/state", saveState());

	QMainWindow::closeEvent(event);
}

void MainWindow::newProject(void)
{
	NewProjectDialog dlg(this);

	if (dlg.exec() == QDialog::Rejected)
		return;

	setupProject(dlg.path());
}

void MainWindow::openProject(void)
{
	QString path = QFileDialog::getExistingDirectory(this, "Open Project Directory");
	setupProject(path);
}

bool MainWindow::setupProject(const QString &path)
{
	QDir dir(path);
	if (!dir.exists() && !dir.mkpath(path)) {
		QMessageBox::warning(this, "Warning", "Cannot create project directory: " + path);
		return false;
	}

	ProjectPtr project(new Project());
	if (!project) {
		QMessageBox::critical(this, "Error", "Cannot create project.");
		return false;
	}

	if (!project->init(path.toStdString().c_str())) {
		QMessageBox::critical(this, "Error", "Failed to initialize project.");
		return false;
	}

	m_projectPath = path;
	m_project = project;

	return true;
}
