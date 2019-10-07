//
//
#include <QApplication>
#include <QCloseEvent>
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

	m_fileNew = m_fileMenu->addAction("New...");
	connect(m_fileNew, &QAction::triggered, this, &MainWindow::fileNew);

	m_fileOpen = m_fileMenu->addAction("Open...");
	connect(m_fileOpen, &QAction::triggered, this, &MainWindow::fileOpen);

	m_fileClose = m_fileMenu->addAction("Close");
	connect(m_fileClose, &QAction::triggered, this, &MainWindow::fileClose);

	m_fileSave = m_fileMenu->addAction("Save");
	connect(m_fileSave, &QAction::triggered, this, &MainWindow::fileSave);

	m_fileSaveAs = m_fileMenu->addAction("Save As...");
	connect(m_fileSaveAs, &QAction::triggered, this, &MainWindow::fileSaveAs);

	m_fileMenu->addSeparator();

	m_fileNewProject = m_fileMenu->addAction("New Project...");
	connect(m_fileNewProject, &QAction::triggered, this, &MainWindow::fileNewProject);

	m_fileOpenProject = m_fileMenu->addAction("Open Project...");
	connect(m_fileOpenProject, &QAction::triggered, this, &MainWindow::fileOpenProject);

	m_fileCloseProject = m_fileMenu->addAction("Close Project");
	connect(m_fileCloseProject, &QAction::triggered, this, &MainWindow::fileCloseProject);

	m_fileSaveProject = m_fileMenu->addAction("Save Project");
	connect(m_fileSaveProject, &QAction::triggered, this, &MainWindow::fileSaveProject);

	m_fileMenu->addSeparator();

	m_fileImport = m_fileMenu->addAction("Import...");
	connect(m_fileImport, &QAction::triggered, this, &MainWindow::fileImport);

	m_fileExport = m_fileMenu->addAction("Export...");
	connect(m_fileExport, &QAction::triggered, this, &MainWindow::fileExport);

	m_fileMenu->addSeparator();

	m_fileQuit = m_fileMenu->addAction("Quit");
	connect(m_fileQuit, &QAction::triggered, this, &MainWindow::fileQuit);

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

	m_projectViewDocker.setParent(this);
	m_projectViewDocker.setObjectName("ProjectView");
	m_projectViewDocker.setWidget(&m_projectView);
	m_projectViewDocker.setWindowTitle("Project");
	addDockWidget(Qt::LeftDockWidgetArea, &m_projectViewDocker);
	m_projectView.setContextMenuPolicy(Qt::CustomContextMenu);
	connect(&m_projectView, &QWidget::customContextMenuRequested, this, &MainWindow::projectContextMenuRequested);
	m_projectContextMenu.addAction(m_fileNew);
	m_projectContextMenu.addAction(m_fileImport);
	m_projectContextMenu.addAction(m_fileExport);

	m_consoleViewDocker.setParent(this);
	m_consoleViewDocker.setObjectName("ConsoleView");
	m_consoleViewDocker.setWidget(&m_consoleView);
	m_consoleViewDocker.setWindowTitle("Console");
	addDockWidget(Qt::BottomDockWidgetArea, &m_consoleViewDocker);

	statusBar()->showMessage("Ready");

	return true;
}

void MainWindow::projectContextMenuRequested(const QPoint &pos)
{
	m_projectContextMenu.popup(m_projectView.mapToGlobal(pos));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QSettings settings;

	settings.setValue("MainWindow/geometry", saveGeometry());
	settings.setValue("MainWindow/state", saveState());

	QMainWindow::closeEvent(event);
}

void MainWindow::fileNew(void)
{
}

void MainWindow::fileOpen(void)
{
}

void MainWindow::fileClose(void)
{
}

void MainWindow::fileSave(void)
{
}

void MainWindow::fileSaveAs(void)
{
}

void MainWindow::fileNewProject(void)
{
	NewProjectDialog dlg(this);

	if (dlg.exec() == QDialog::Accepted)
		setupProject(dlg.path());
}

void MainWindow::fileOpenProject(void)
{
	QString path = QFileDialog::getExistingDirectory(this, "Open Project Directory");
	if (!path.isEmpty())
		setupProject(path);
}

void MainWindow::fileCloseProject(void)
{
}

void MainWindow::fileSaveProject(void)
{
}

void MainWindow::fileImport(void)
{
}

void MainWindow::fileExport(void)
{
}

void MainWindow::fileQuit(void)
{
	close();
}

bool MainWindow::setupProject(const QString &path)
{
	QDir dir(path);

	m_projectView.setProject(NULL);

	if (!dir.exists() && !dir.mkpath(path)) {
		QMessageBox::critical(this, "Error", "Cannot create project directory: " + path);
		return false;
	}

	ProjectPtr project(new Project());
	if (!project) {
		QMessageBox::critical(this, "Error", "Cannot create project.");
		return false;
	}

	if (!project->setup(path)) {
		QMessageBox::critical(this, "Error", "Failed to initialize project.");
		return false;
	}

	m_projectPath = path;
	m_project = project;

	m_projectView.setProject(m_project.data());

	return true;
}

