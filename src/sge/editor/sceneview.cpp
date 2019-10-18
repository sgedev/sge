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
	m_rc.init(QOpenGLContext::currentContext());
}

void SceneView::paintGL(void)
{
	if (m_project == Q_NULLPTR)
		return;

	m_view.clear();
	m_project->draw(&m_view);

	m_rc.clear();
	m_rc.render(&m_view);
}

void SceneView::resizeGL(int w, int h)
{
	m_view.setViewport(0, 0, w, h);
}

SGE_EDITOR_END

