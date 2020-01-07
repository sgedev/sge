//
//
#include <sge/camera.hpp>

SGE_BEGIN

Camera::Camera(Node *parent)
	: Node(parent)
{
	m_rootNode = parent;

	if (m_rootNode != Q_NULLPTR) {
		while (m_rootNode->parent() != Q_NULLPTR)
			m_rootNode = qobject_cast<Node *>(m_rootNode->parent());
	}
}

Camera::~Camera(void)
{
}

void Camera::update(float elapsed)
{
	if (m_rootNode == Q_NULLPTR)
		return;

	m_view.reset();
	m_view.setViewMatrix(-globalTransform());

	drawNode(m_rootNode, elapsed);
}

void Camera::drawNode(Node *node, float elapsed)
{
	if (static_cast<Node *>(this) == node)
		return;
}

SGE_END

