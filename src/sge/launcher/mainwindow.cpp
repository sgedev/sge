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
	m_dbfs.init(filename);
	m_db.load(&m_dbfs);
	m_game.init(&m_db);

	return true;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_frameTimer) {
		m_game.update(16.6f);
		update();
	}
}

void MainWindow::initializeGL(void)
{
	m_rc.init();
}

void MainWindow::paintGL(void)
{
	m_view.clear();
	m_game.draw(&m_view);

	m_rc.clear();
	m_rc.render(&m_view);
}

void MainWindow::resizeGL(int w, int h)
{
	m_view.setViewport(0, 0, w, h);
}

SGE_LAUNCHER_END

