//
//
#include "mainwindow.hpp"

MainWindow::MainWindow(void)
{
	m_mdiArea.setParent(this);
}

MainWindow::~MainWindow(void)
{
}

bool MainWindow::Init(void)
{
	setCentralWidget(&m_mdiArea);

	m_mdiArea.setViewMode(QMdiArea::TabbedView);
	m_mdiArea.setTabsClosable(true);
	m_mdiArea.setTabsMovable(true);
	//m_mdiArea.setDocumentMode(true);

	m_sceneViewSubWindow.setWidget(&m_sceneView);
	m_mdiArea.addSubWindow(&m_sceneViewSubWindow);
	m_sceneViewSubWindow.showMaximized();

	m_projectViewDocker.setWidget(&m_projectView);
	addDockWidget(Qt::LeftDockWidgetArea, &m_projectViewDocker);

	m_consoleViewDocker.setWidget(&m_consoleView);
	addDockWidget(Qt::BottomDockWidgetArea, &m_consoleViewDocker);

	return true;
}
