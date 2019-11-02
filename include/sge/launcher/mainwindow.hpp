//
//
#ifndef SGE_LAUNCHER_MAINWINDOW_HPP
#define SGE_LAUNCHER_MAINWINDOW_HPP

#include <QTimerEvent>
#include <QOpenGLWindow>

#include <sge/launcher/common.hpp>
#include <sge/launcher/game.hpp>

SGE_LAUNCHER_BEGIN

class MainWindow: public QOpenGLWindow {
	Q_OBJECT

public:
	MainWindow(void);
	virtual ~MainWindow(void);

protected:
    void timerEvent(QTimerEvent *event) override;
	void initializeGL(void) override;
	void paintGL(void) override;
	void resizeGL(int w, int h) override;

private:
	int m_frameTimer;
	ttvfs::Root m_root;
	Game m_game;
	Renderer::Context m_rc;
	Renderer::View m_view;
};

SGE_LAUNCHER_END

#endif // SGE_LAUNCHER_MAINWINDOW_HPP

