//
//
#ifndef SGE_EDITOR_SCENEVIEW_HPP
#define SGE_EDITOR_SCENEVIEW_HPP

#include <GL/gl3w.h>
#include <glex.hpp>
#include <sge.hpp>

#include <QOpenGLContext>
#include <QOpenGLWidget>

class Project;

class SceneView: public QOpenGLWidget {
	Q_OBJECT

public:
	SceneView(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
	virtual ~SceneView(void);

public:
	bool attachProject(Project *project);
	Project *project(void);

protected:
	void initializeGL(void) override;
	void paintGL(void) override;
	void resizeGL(int w, int h) override;

private:
	Project *m_project;
	GL3WProcs m_gl3w;
	GLEX::Context m_glex;
	SGE::View m_view;
};

inline Project *SceneView::project(void)
{
	return m_project;
}

#endif // SGE_EDITOR_SCENEVIEW_HPP
