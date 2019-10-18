//
//
#include <sge/launcher/gamewidget.hpp>

SGE_LAUNCHER_BEGIN

GameWidget::GameWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
}

GameWidget::~GameWidget(void)
{
}

void GameWidget::frame(float elapsed)
{
	m_game.update(elapsed);
	update();
}

void GameWidget::initializeGL(void)
{
	m_game.init(&m_root);
	m_rc.init();
}

void GameWidget::paintGL(void)
{
	m_view.clear();
	m_game.draw(&m_view);

	m_rc.clear();
	m_rc.render(&m_view);
}

void GameWidget::resizeGL(int w, int h)
{
	m_view.setViewport(0, 0, w, h);
}

SGE_LAUNCHER_END

