//
//
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <SGL/Scene.hpp>
#include <SGL/OpenGLWinodw.hpp>
#include <SGL/OpenGLRenderer.hpp>
#include <SGL/BulletPhysics.hpp>

class MainWindow: public SGL::OpenGLWindow {
public:
	MainWindow(void);
	virtual ~MainWindow(void);

protected:
	void paint(void) override;

private:
	SGL::Scene m_scene;
	SGL::OpenGLRenderer m_renderer;
};

#endif // MAINWINDOW_HPP

