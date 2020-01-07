//
//
#include <sge/scene.hpp>

SGE_BEGIN

Scene::Scene(void)
{
}

Scene::~Scene(void)
{
}

void Scene::update(float elapsed)
{
	updateNode(&m_rootNode, elapsed);
}

void Scene::updateNode(Node *node, float elapsed)
{
	const QObjectList &children = node->children();
	for (auto it(children.begin()); it != children.end(); ++it) {
		Node *child = qobject_cast<Node *>(*it);
		child->update(elapsed);
		updateNode(child, elapsed);
	}
}

SGE_END

