//
//
#include <QLayout>
#include <QTimerEvent>

#include <sge/launcher/mainwindow.hpp>

SGE_LAUNCHER_BEGIN

MainWindow::MainWindow(void)
{
	setWindowTitle("SGE Launcher");
	setCentralWidget(&m_gameWidget);
	resize(800, 600);
	setMinimumSize(size());
	setMaximumSize(size());
	m_frameTimer = startTimer(16, Qt::PreciseTimer);
}

MainWindow::~MainWindow(void)
{
	if (m_frameTimer > 0)
		killTimer(m_frameTimer);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_frameTimer)
		m_gameWidget.frame(16);
}

SGE_LAUNCHER_END

