//
//
#include <QVector3D>
#include <QMatrix4x4>

#include <sge/renderer.hpp>

SGE_BEGIN

Renderer::Renderer(QObject *parent)
	: QObject(parent)
	, m_scene(Q_NULLPTR)
	, m_context(Q_NULLPTR)
	, m_gl(Q_NULLPTR)
{
}

Renderer::~Renderer(void)
{
}

bool Renderer::init(Scene *scene)
{
	Q_ASSERT(scene != Q_NULLPTR);
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

	connect(scene, &Scene::nodeAdded, this, &Renderer::nodeAdded);
	connect(scene, &Scene::nodeRemoved, this, &Renderer::nodeRemoved);

	m_scene = scene;

	return true;
}

void Renderer::reset(void)
{
	Q_ASSERT(m_context == QOpenGLContext::currentContext());
	Q_ASSERT(m_gl != Q_NULLPTR);

	// TODO
}

void Renderer::draw(void)
{
	Q_ASSERT(m_context == QOpenGLContext::currentContext());
	Q_ASSERT(m_gl != Q_NULLPTR);

	for (auto it(m_cameraList.begin()); it != m_cameraList.end(); ++it) {
		Camera *camera = *it;
		Q_ASSERT(camera != Q_NULLPTR);
		if (camera->isEnabled()) {
			m_scene->buildView(m_view, camera);
			renderView(m_view);
		}
	}
}

void Renderer::renderView(View &view)
{
}

void Renderer::nodeAdded(Node *node)
{
	Camera *camera = qobject_cast<Camera *>(node);
	if (camera != Q_NULLPTR) {
		m_cameraList.push_back(camera);
		return;
	}
}

void Renderer::nodeRemoved(Node *node)
{
	Camera *camera = qobject_cast<Camera *>(node);
	if (camera != Q_NULLPTR) {
		m_cameraList.removeOne(camera);
		return;
	}

	World *world = qobject_cast<World *>(node);
	if (world != Q_NULLPTR) {
		return;
	}

	Entity *entity = qobject_cast<Entity *>(node);
	if (entity != Q_NULLPTR) {
		return;
	}
}

SGE_END

