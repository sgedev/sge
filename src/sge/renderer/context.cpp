//
//
#include <QVector3D>
#include <QMatrix4x4>

#include <sge/renderer/context.hpp>

SGE_RENDERER_BEGIN

Context::Context(QObject *parent)
	: QObject(parent)
{
}

Context::~Context(void)
{
}

bool Context::init(QOpenGLContext *context)
{
	m_context = context ? context : QOpenGLContext::currentContext();
	if (m_context == Q_NULLPTR)
		return false;

	m_gl = m_context->versionFunctions<OpenGLFunctions>();
	if (m_gl == Q_NULLPTR)
		return false;

	m_gl->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// TODO

	return true;
}

void Context::clear(void)
{
	Q_ASSERT(m_context == QOpenGLContext::currentContext());
	Q_ASSERT(m_gl != Q_NULLPTR);

	m_gl->glDisable(GL_SCISSOR_TEST);
	m_gl->glClear(GL_COLOR_BUFFER_BIT);
}

void Context::render(View *view)
{
	Q_ASSERT(view != Q_NULLPTR);
	Q_ASSERT(m_context == QOpenGLContext::currentContext());
	Q_ASSERT(m_gl != Q_NULLPTR);

	QRect vp = view->viewport();
	if (vp.isEmpty())
		return;

	m_gl->glViewport(vp.x(), vp.y(), vp.width(), vp.height());
	m_gl->glScissor(vp.x(), vp.y(), vp.width(), vp.height());

	if (view->isClearEnabled()) {
		QVector3D clearColor = view->clearColor();
		m_gl->glClearColor(clearColor.x(), clearColor.y(), clearColor.z(), 1.0f);
		m_gl->glClear(GL_COLOR_BUFFER_BIT);
	}

	QMatrix4x4 viewMatrix = view->viewMatrix();
	QMatrix4x4 projectMatrix = view->projectMatrix();
	// TODO
}

SGE_RENDERER_END

