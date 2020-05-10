//
//
#include "MainWindow.hpp"

MainWindow::MainWindow(void)
{
}

MainWindow::~MainWindow(void)
{
}

bool MainWindow::init(const char *name, int x, int y, int width, int height)
{
	if (!SGL::OpenGLWindow::init(name, x, y, width, height, 0))
		return false;

	return true;
}

void MainWindow::paint(void)
{
}

bool MainWindow::initGL(void)
{
	m_renderer.init();

	m_scene.addListener(&m_renderer);

	return true;
}
