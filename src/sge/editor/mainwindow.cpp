//
//
#include <QDebug>
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

#include <sge/editor/newprojectdialog.hpp>
#include <sge/editor/newfolderdialog.hpp>
#include <sge/editor/newscriptdialog.hpp>
#include <sge/editor/newscenedialog.hpp>
#include <sge/editor/mainwindow.hpp>

SGE_EDITOR_BEGIN

MainWindow::MainWindow(void)
{
	QSettings settings;
	restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
	restoreState(settings.value("MainWindow/state").toByteArray());

	setWindowTitle("SGE Editor");

	initMainMenu();
	initToolBars();

	setCentralWidget(&m_mdiArea);
	m_mdiArea.setViewMode(QMdiArea::TabbedView);
	m_mdiArea.setTabsClosable(true);
	m_mdiArea.setTabsMovable(true);

	initProjectView();
	initConsoleView();

	statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow(void)
{
}

void MainWindow::projectOpenItem(const QModelIndex &index)
{
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

void MainWindow::fileNewProject(void)
{
	NewProjectDialog dlg(this);

	if (dlg.exec() == QDialog::Accepted)
		setupProject(dlg.path());
}

void MainWindow::fileNewFolder(void)
{
	NewFolderDialog dlg(this);

	dlg.exec();
}

void MainWindow::fileNewScript(void)
{
	NewScriptDialog dlg(this);

	dlg.exec();
}

void MainWindow::fileNewScene(void)
{
	NewSceneDialog dlg(this);

	dlg.exec();
}

void MainWindow::fileOpen(void)
{
	QString path = QFileDialog::getExistingDirectory(this, "Open Project Directory");
	if (!path.isEmpty())
		setupProject(path);
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

void MainWindow::gameBuild(void)
{
}

void MainWindow::gameClean(void)
{
}

void MainWindow::gameRun(void)
{
}

void MainWindow::helpAbout(void)
{
	QString text = "SGE Editor v0.1";

	QMessageBox::about(this, "About", text);
}

void MainWindow::helpAboutQt(void)
{
	QMessageBox::aboutQt(this);
}

bool MainWindow::setupProject(const QString &path)
{
	QDir dir(path);

	m_fileNewFolder->setDisabled(true);
	m_fileNewScene->setDisabled(true);
	m_fileNewScript->setDisabled(true);

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

	if (!project->create(dir)) {
		QMessageBox::critical(this, "Error", "Failed to initialize project.");
		return false;
	}

	m_projectPath = path;
	m_project = project;

	m_projectView.setProject(m_project.data());

	m_fileNewFolder->setEnabled(true);
	m_fileNewScene->setEnabled(true);
	m_fileNewScript->setEnabled(true);
	m_fileImport->setEnabled(true);
	m_fileExport->setEnabled(true);

	return true;
}

void MainWindow::initMainMenu(void)
{
	m_fileMenu = menuBar()->addMenu("&File");

	m_fileNewMenu = m_fileMenu->addMenu("New");

	m_fileNewProject = m_fileNewMenu->addAction("Project...");
	connect(m_fileNewProject, &QAction::triggered, this, &MainWindow::fileNewProject);

	m_fileNewFolder = m_fileNewMenu->addAction("Folder...");
	m_fileNewFolder->setDisabled(true);
	connect(m_fileNewFolder, &QAction::triggered, this, &MainWindow::fileNewFolder);

	m_fileNewScene = m_fileNewMenu->addAction("Scene...");
	m_fileNewScene->setDisabled(true);
	connect(m_fileNewScene, &QAction::triggered, this, &MainWindow::fileNewScene);

	m_fileNewScript = m_fileNewMenu->addAction("Script...");
	m_fileNewScript->setDisabled(true);
	connect(m_fileNewScript, &QAction::triggered, this, &MainWindow::fileNewScript);

	m_fileOpen = m_fileMenu->addAction("Open...");
	connect(m_fileOpen, &QAction::triggered, this, &MainWindow::fileOpen);

	m_fileClose = m_fileMenu->addAction("Close");
	connect(m_fileClose, &QAction::triggered, this, &MainWindow::fileClose);

	m_fileSave = m_fileMenu->addAction("Save");
	connect(m_fileSave, &QAction::triggered, this, &MainWindow::fileSave);

	m_fileSaveAs = m_fileMenu->addAction("Save As...");
	connect(m_fileSaveAs, &QAction::triggered, this, &MainWindow::fileSaveAs);

	m_fileMenu->addSeparator();

	m_fileImport = m_fileMenu->addAction("Import...");
	m_fileImport->setDisabled(true);
	connect(m_fileImport, &QAction::triggered, this, &MainWindow::fileImport);

	m_fileExport = m_fileMenu->addAction("Export...");
	m_fileExport->setDisabled(true);
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

	m_gameMenu = menuBar()->addMenu("&Game");

	m_gameBuild = m_gameMenu->addAction("&Build");
	connect(m_gameBuild, &QAction::triggered, this, &MainWindow::gameBuild);

	m_gameClean = m_gameMenu->addAction("&Clean");
	connect(m_gameClean, &QAction::triggered, this, &MainWindow::gameClean);

	m_gameRun = m_gameMenu->addAction("&Run");
	connect(m_gameRun, &QAction::triggered, this, &MainWindow::gameRun);

	m_toolMenu = menuBar()->addMenu("&Tool");
	m_toolOptions = m_toolMenu->addAction("&Options");

	m_helpMenu = menuBar()->addMenu("&Help");

	m_helpAbout = m_helpMenu->addAction("&About...");
	connect(m_helpAbout, &QAction::triggered, this, &MainWindow::helpAbout);

	m_helpAboutQt = m_helpMenu->addAction("About &Qt...");
	connect(m_helpAboutQt, &QAction::triggered, this, &MainWindow::helpAboutQt);
}

void MainWindow::initToolBars(void)
{
	m_mainToolBar.setObjectName("MainToolBar");
	m_mainToolBar.addAction(m_fileNewProject);
	m_mainToolBar.addAction(m_fileNewFolder);
	m_mainToolBar.addAction(m_fileNewScene);
	m_mainToolBar.addAction(m_fileNewScript);
	addToolBar(&m_mainToolBar);

	m_viewToolBar.setObjectName("ViewToolBar");
	m_viewToolBar.addAction(m_viewProject);
	m_viewToolBar.addAction(m_viewConsole);
	addToolBar(&m_viewToolBar);
}

void MainWindow::initProjectView(void)
{
	restoreDockWidget(&m_projectViewDocker);

	m_projectViewDocker.setObjectName("ProjectView");
	m_projectViewDocker.setWindowTitle("Project");
	m_projectViewDocker.setWidget(&m_projectView);

	m_projectView.setContextMenuPolicy(Qt::CustomContextMenu);
	m_projectView.setParent(&m_projectViewDocker);

	connect(&m_projectView, &QAbstractItemView::doubleClicked, this, &MainWindow::projectOpenItem);
	connect(&m_projectView, &QWidget::customContextMenuRequested, this, &MainWindow::projectContextMenuRequested);

	m_projectContextNewMenu = m_projectContextMenu.addMenu("New");
	m_projectContextNewMenu->addAction(m_fileNewFolder);
	m_projectContextNewMenu->addAction(m_fileNewScene);
	m_projectContextNewMenu->addAction(m_fileNewScript);

	m_projectContextMenu.addAction(m_fileImport);
	m_projectContextMenu.addAction(m_fileExport);

	addDockWidget(Qt::LeftDockWidgetArea, &m_projectViewDocker);
}

void MainWindow::initConsoleView(void)
{
	restoreDockWidget(&m_consoleViewDocker);

	m_consoleViewDocker.setObjectName("ConsoleView");
	m_consoleViewDocker.setWindowTitle("Console");
	m_consoleViewDocker.setWidget(&m_consoleView);

	addDockWidget(Qt::BottomDockWidgetArea, &m_consoleViewDocker);
}

SGE_EDITOR_END

