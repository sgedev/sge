//
//
#ifndef SGE_EDITOR_MAINWINDOW_HPP
#define SGE_EDITOR_MAINWINDOW_HPP

#include <QMainWindow>
#include <QMdiArea>
#include <QDockWidget>
#include <QMdiSubWindow>
#include <QPoint>
#include <QSharedPointer>
#include <QMenu>
#include <QToolBar>
#include <QDir>

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

protected slots:
	void projectOpenFile(const QString &filename);
	void projectFolderContextMenuRequested(const QPoint &pos);
	void projectFileContextMenuRequested(const QPoint &pos);
	void projectOtherContextMenuRequested(const QPoint &pos);

protected:
	void closeEvent(QCloseEvent *event) override;

protected:
	void fileNewProject(void);
	void fileNewFolder(void);
	void fileNewScene(void);
	void fileNewScript(void);
	void fileOpen(void);
	void fileClose(void);
	void fileSave(void);
	void fileSaveAs(void);
	void fileImport(void);
	void fileExport(void);
	void fileQuit(void);
	void gameBuild(void);
	void gameClean(void);
	void gameRun(void);
	void helpAbout(void);
	void helpAboutQt(void);
	bool setupProject(const QString &path);

private:
	void initMainMenu(void);
	void initToolBars(void);
	void initProjectView(void);
	void initConsoleView(void);
	void openScript(const QString &filename);
	void openScene(const QString &filename);

private:
	QMenu *m_fileMenu;
	QMenu *m_fileNewMenu;
	QAction *m_fileNewProject;
	QAction *m_fileNewFolder;
	QAction *m_fileNewScene;
	QAction *m_fileNewScript;
	QAction *m_fileOpen;
	QAction *m_fileClose;
	QAction *m_fileSave;
	QAction *m_fileSaveAs;
	QAction *m_fileOpenProject;
	QAction *m_fileCloseProject;
	QAction *m_fileSaveProject;
	QAction *m_fileImport;
	QAction *m_fileExport;
	QAction *m_fileQuit;

	QMenu *m_editMenu;
	QAction *m_editCut;
	QAction *m_editCopy;
	QAction *m_editPaste;

	QMenu *m_viewMenu;
	QAction *m_viewProject;
	QAction *m_viewConsole;

	QMenu *m_gameMenu;
	QAction *m_gameBuild;
	QAction *m_gameClean;
	QAction *m_gameRun;

	QMenu *m_toolMenu;
	QAction *m_toolOptions;

	QMenu *m_helpMenu;
	QAction *m_helpAbout;
	QAction *m_helpAboutQt;

	QToolBar m_mainToolBar;
	QToolBar m_viewToolBar;

	QMdiArea m_mdiArea;

	QDockWidget m_projectViewDocker;
	ProjectView m_projectView;
	QMenu m_projectContextMenu;
	QMenu *m_projectContextNewMenu;
	QAction *m_projectContextNew;

	QDockWidget m_consoleViewDocker;
	ConsoleView m_consoleView;

	typedef QSharedPointer<Project> ProjectPtr;
	QString m_projectPath;
	ProjectPtr m_project;
};

#endif // SGE_EDITOR_MAINWINDOW_HPP

