//
//
#ifndef SGE_EDITOR_SCENEVIEW_HPP
#define SGE_EDITOR_SCENEVIEW_HPP

#include <QEvent>
#include <QOpenGLContext>
#include <QOpenGLWidget>
#include <QTabWidget>

#include <sge/renderer.hpp>
#include <sge/editor/common.hpp>

SGE_EDITOR_BEGIN

class Project;

class SceneView: public QOpenGLWidget {
	Q_OBJECT

public:
	SceneView(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
	virtual ~SceneView(void);

public:
	bool setProject(Project *project);
	Project *project(void);

protected:
	bool event(QEvent *evt) override;
	void initializeGL(void) override;
	void paintGL(void) override;
	void resizeGL(int w, int h) override;

private:
	QTabWidget m_tab;
	Project *m_project;
	Renderer::Context m_rc;
	Renderer::View m_view;
};

inline Project *SceneView::project(void)
{
	return m_project;
}

SGE_EDITOR_END

#endif // SGE_EDITOR_SCENEVIEW_HPP
