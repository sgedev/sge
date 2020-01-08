//
//
#include <sge/scene.hpp>

SGE_BEGIN

Scene::Scene(QObject *parent)
	: QObject(parent)
	, m_rootNode(Q_NULLPTR) // root node's parent must be NULL.
{
	connect(&m_rootNode, &Node::childAdded, this, &Scene::childAdded);
	connect(&m_rootNode, &Node::childRemoved, this, &Scene::childRemoved);
}

Scene::~Scene(void)
{
}

bool Scene::init(void)
{
	return true;
}

void Scene::update(float elapsed)
{
	m_rootNode.update(elapsed);
}

void Scene::buildView(View &view, Camera *from) const
{
	Q_ASSERT(from != Q_NULLPTR);
	buildNodeView(&m_rootNode, view, from);
}

void Scene::buildNodeView(const Node *node, View &view, Camera *from) const
{
	const QObjectList &children = node->children();
	for (auto it(children.begin()); it != children.end(); ++it) {
		const Node *node = qobject_cast<const Node *>(*it);
		if (node = Q_NULLPTR)
			continue;
		if (!node->isVisibled())
			continue;
		if (from->canSee(node))
			node->draw(view);
		buildNodeView(node, view, from);
	}
}

void Scene::childAdded(Node *child)
{
	connect(child, &Node::childAdded, this, &Scene::childAdded);
	connect(child, &Node::childRemoved, this, &Scene::childRemoved);

	nodeAdded(child);
}

void Scene::childRemoved(Node *child)
{
	nodeRemoved(child);
}

SGE_END

