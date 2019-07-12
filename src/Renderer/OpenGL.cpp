//
//
#include <SGE/Renderer/OpenGL.h>

SGE_RENDERER_BEGIN

OpenGL::OpenGL(QObject *parent)
	: Base(parent)
	, m_surface(Q_NULLPTR)
	, m_context(Q_NULLPTR)
	, m_gl(Q_NULLPTR)
{
}

OpenGL::~OpenGL(void)
{
	if (m_surface == Q_NULLPTR)
		return;
}

bool OpenGL::create(QSurface *surface, QOpenGLContext *context)
{
	Q_ASSERT(surface != Q_NULLPTR);
	Q_ASSERT(context != Q_NULLPTR);

	Q_ASSERT(m_surface == Q_NULLPTR);
	Q_ASSERT(m_context == Q_NULLPTR);
	Q_ASSERT(m_gl == Q_NULLPTR);

	if (surface->size().isNull() || !surface->supportsOpenGL())
		return false;

	if (context->isValid())
		return false;

	if (!Base::init())
		return false;

	QSurface *old_surface = Q_NULLPTR;
	QOpenGLContext *old_context = QOpenGLContext::currentContext();
	if (old_context != Q_NULLPTR)
		old_surface = old_context->surface();

	if (!context->makeCurrent(surface))
		return false;

	m_gl = context->versionFunctions<Functions>();
	if (m_gl == Q_NULLPTR) {
		context->doneCurrent();
		if (old_context != Q_NULLPTR)
			old_context->makeCurrent(old_surface);
		return false;
	}

	m_gl->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	context->doneCurrent();

	if (old_context != Q_NULLPTR)
		old_context->makeCurrent(old_surface);

	m_context = context;
	m_surface = surface;

	return true;
}

void OpenGL::reset(void)
{
	Q_ASSERT(m_surface != Q_NULLPTR);
	Q_ASSERT(m_context != Q_NULLPTR);
	Q_ASSERT(m_gl != Q_NULLPTR);
}

void OpenGL::render(void)
{
	Q_ASSERT(m_surface != Q_NULLPTR);
	Q_ASSERT(m_context != Q_NULLPTR);
	Q_ASSERT(m_context == QOpenGLContext::currentContext());
	Q_ASSERT(m_gl != Q_NULLPTR);

	const QRect &vp = viewport();

	if (vp.isNull())
		return;

	m_gl->glViewport(vp.x(), vp.y(), vp.width(), vp.height());
}

SGE_RENDERER_END

