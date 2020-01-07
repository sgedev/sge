//
//
#include <sge/node.hpp>

SGE_BEGIN

Node::Node(Node *parent)
	: QObject(parent)
{
}

Node::~Node(void)
{
}

QMatrix4x4 Node::globalTransform(void) const
{
	QMatrix4x4 transform = m_transform;

	Node *p = qobject_cast<Node *>(parent());
	while (p != Q_NULLPTR) {
		transform = p->m_transform * transform;
		p = qobject_cast<Node *>(p->parent());
	}

	return transform;
}

void Node::update(float elapsed)
{
}

SGE_END

