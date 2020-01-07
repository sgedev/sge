//
//
#include <QVector3D>
#include <QMatrix4x4>

#include <sge/renderer.hpp>

SGE_BEGIN

Renderer::Renderer(void)
	: m_context(Q_NULLPTR)
	, m_gl(Q_NULLPTR)
{
}

Renderer::~Renderer(void)
{
}

bool Renderer::init(void)
{
	Q_ASSERT(m_context == Q_NULLPTR);
	Q_ASSERT(m_gl == Q_NULLPTR);

	m_context = QOpenGLContext::currentContext();
	if (m_context == Q_NULLPTR)
		return false;

	m_gl = m_context->versionFunctions<OpenGLFunctions>();
	if (m_gl == Q_NULLPTR)
		return false;

	m_gl->initializeOpenGLFunctions();

	m_gl->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	return true;
}

void Renderer::reset(void)
{
	Q_ASSERT(m_context == QOpenGLContext::currentContext());
	Q_ASSERT(m_gl != Q_NULLPTR);

	// TODO
}

void Renderer::draw(View &view)
{
	Q_ASSERT(m_context == QOpenGLContext::currentContext());
	Q_ASSERT(m_gl != Q_NULLPTR);

	if (view.isClearEnabled()) {
		const QColor &color = view.clearColor();
		m_gl->glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
		m_gl->glClear(GL_COLOR_BUFFER_BIT);
	}
}

SGE_END

