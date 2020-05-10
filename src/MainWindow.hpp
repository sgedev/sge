//
//
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <SGL/Scene.hpp>
#include <SGL/OpenGLWindow.hpp>
#include <SGL/OpenGLRenderer.hpp>
#include <SGL/BulletPhysics.hpp>

class MainWindow: public SGL::OpenGLWindow {
public:
	MainWindow(void);
	virtual ~MainWindow(void);

public:
	bool init(const char *name, int x, int y, int width, int height);

protected:
	void paint(void) override;
	bool initGL(void) override;

private:
	SGL::Scene m_scene;
	SGL::OpenGLRenderer m_renderer;
};

#endif // MAINWINDOW_HPP

