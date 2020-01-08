//
//
#include <QEvent>
#include <QChildEvent>

#include <sge/node.hpp>

SGE_BEGIN

Node::Node(QObject *parent)
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

void Node::childEvent(QChildEvent *event)
{
	QObject::childEvent(event);

	Node *child = qobject_cast<Node *>(event->child());
	if (child == Q_NULLPTR)
		return;

	switch (event->type()) {
	case QEvent::ChildAdded:
		m_childrenCache.append(child);
		childAdded(child);
		break;
	case QEvent::ChildRemoved:
		m_childrenCache.removeOne(child);
		childRemoved(child);
		break;
	}
}

void Node::updateNode(float elapsed)
{
	for (auto it(m_childrenCache.begin()); it != m_childrenCache.end(); ++it) {
		Node *child = qobject_cast<Node *>(*it);
		Q_ASSERT(child != Q_NULLPTR);
		child->update(elapsed);
	}
}

void Node::drawNode(View &view) const
{
	for (auto it(m_childrenCache.begin()); it != m_childrenCache.end(); ++it) {
		Node *child = qobject_cast<Node *>(*it);
		Q_ASSERT(child != Q_NULLPTR);
		child->draw(view);
	}
}

SGE_END

