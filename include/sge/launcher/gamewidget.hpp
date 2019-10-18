//
//
#ifndef SGE_LAUNCHER_GAMEWIDGET_HPP
#define SGE_LAUNCHER_GAMEWIDGET_HPP

#include <QOpenGLWidget>

#include <sge/renderer.hpp>
#include <sge/launcher/common.hpp>
#include <sge/launcher/game.hpp>

SGE_LAUNCHER_BEGIN

class GameWidget: public QOpenGLWidget {
	Q_OBJECT

public:
	GameWidget(QWidget *parent = Q_NULLPTR);
	virtual ~GameWidget(void);

public:
	void frame(float elapsed);

protected:
	void initializeGL(void) override;
	void paintGL(void) override;
	void resizeGL(int w, int h) override;

private:
	ttvfs::Root m_root;
	Game m_game;
	Renderer::Context m_rc;
	Renderer::View m_view;
};

SGE_LAUNCHER_END

#endif // SGE_LAUNCHER_GAMEWIDGET_HPP

