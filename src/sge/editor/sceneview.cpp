//
//
#include <sge/editor/project.hpp>
#include <sge/editor/sceneview.hpp>

SGE_EDITOR_BEGIN

SceneView::SceneView(QWidget *parent, Qt::WindowFlags flags)
	: QOpenGLWidget(parent, flags)
	, m_project(Q_NULLPTR)
{
	setWindowTitle("SceneView");
}

SceneView::~SceneView(void)
{
}

bool SceneView::setProject(Project *project)
{
	m_project = project;

	return true;
}

bool SceneView::event(QEvent *evt)
{
	return QOpenGLWidget::event(evt);
}

void SceneView::initializeGL(void)
{
}

void SceneView::paintGL(void)
{
	if (m_project == Q_NULLPTR)
		return;
}

void SceneView::resizeGL(int w, int h)
{
}

SGE_EDITOR_END

