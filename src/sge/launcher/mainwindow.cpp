//
//
#include <QLayout>
#include <QTimerEvent>

#include <sge/launcher/mainwindow.hpp>

SGE_LAUNCHER_BEGIN

MainWindow::MainWindow(void)
{
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

bool MainWindow::init(const QString &filename)
{
	return true;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_frameTimer) {
		//m_game.update(16.6f);
		update();
	}
}

void MainWindow::initializeGL(void)
{

}

void MainWindow::paintGL(void)
{
}

void MainWindow::resizeGL(int w, int h)
{
}

SGE_LAUNCHER_END

