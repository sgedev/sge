//
//
#include "project.hpp"
#include "sceneview.hpp"

static GL3WglProc GetOpenGLProcAddress(const char *proc)
{
	return (GL3WglProc)(QOpenGLContext::currentContext()->getProcAddress(proc));
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

bool SceneView::attachProject(Project *project)
{
	m_project = project;

	return true;
}

void SceneView::initializeGL(void)
{
	gl3wInit(&m_gl3w, GetOpenGLProcAddress);
	gl3wProcs = &m_gl3w;

	m_glex.init();
	m_view.init(&m_glex);
}

void SceneView::paintGL(void)
{
	gl3wProcs = &m_gl3w;

	m_glex.beginFrame();

	if (m_project != Q_NULLPTR)
		m_project->draw(&m_view);

	m_view.render();

	m_glex.endFrame();
}

void SceneView::resizeGL(int w, int h)
{
	gl3wProcs = &m_gl3w;

	glViewport(0, 0, w, h);
}
