//
//
#include "project.hpp"
#include "sceneview.hpp"

static GL3WglProc GetOpenGLProcAddress(const char *proc)
{
	SGE_ASSERT(QOpenGLContext::currentContext() != Q_NULLPTR);
	return (GL3WglProc)QOpenGLContext::currentContext()->getProcAddress(proc);
}

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
	if (m_project != Q_NULLPTR)
		m_project->handleEvent(evt);

	return QOpenGLWidget::event(evt);
}

void SceneView::initializeGL(void)
{
	m_view.init(GetOpenGLProcAddress);
}

void SceneView::paintGL(void)
{
	if (m_project == Q_NULLPTR)
		return;

	m_view.beginFrame();
	m_project->draw(&m_view);
	m_view.endFrame();
}

void SceneView::resizeGL(int w, int h)
{
	m_view.setViewport(0, 0, w, h);
}

