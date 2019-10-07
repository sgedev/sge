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
	void projectContextMenuRequested(const QPoint &pos);

protected:
	void closeEvent(QCloseEvent *event) override;

protected:
	void fileNew(void);
	void fileOpen(void);
	void fileClose(void);
	void fileSave(void);
	void fileSaveAs(void);
	void fileNewProject(void);
	void fileOpenProject(void);
	void fileCloseProject(void);
	void fileSaveProject(void);
	void fileImport(void);
	void fileExport(void);
	void fileQuit(void);
	bool setupProject(const QString &path);

private:
	QMenu *m_fileMenu;
	QAction *m_fileNew;
	QAction *m_fileOpen;
	QAction *m_fileClose;
	QAction *m_fileSave;
	QAction *m_fileSaveAs;
	QAction *m_fileNewProject;
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

	QMenu *m_toolMenu;
	QAction *m_toolOptions;

	QMenu *m_helpMenu;
	QAction *m_helpAbout;

	QMdiArea m_mdiArea;

	QDockWidget m_projectViewDocker;
	ProjectView m_projectView;
	QMenu m_projectContextMenu;
	QAction *m_projectNewObject;
	QAction *m_projectImportObject;
	QAction *m_projectExportObject;

	QDockWidget m_consoleViewDocker;
	ConsoleView m_consoleView;

	typedef QSharedPointer<Project> ProjectPtr;
	QString m_projectPath;
	ProjectPtr m_project;
};

#endif // SGE_EDITOR_MAINWINDOW_HPP

