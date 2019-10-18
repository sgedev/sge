//
//
#ifndef SGE_LAUNCHER_MAINWINDOW_HPP
#define SGE_LAUNCHER_MAINWINDOW_HPP

#include <QMainWindow>

#include <sge/launcher/common.hpp>
#include <sge/launcher/gamewidget.hpp>

SGE_LAUNCHER_BEGIN

class MainWindow: public QMainWindow {
	Q_OBJECT

public:
	MainWindow(void);
	virtual ~MainWindow(void);

protected:
    void timerEvent(QTimerEvent *event);

private:
	GameWidget m_gameWidget;
	int m_frameTimer;
};

SGE_LAUNCHER_END

#endif // SGE_LAUNCHER_MAINWINDOW_HPP

