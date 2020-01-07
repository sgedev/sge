//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <QList>
#include <QObject>

#include <sge/common.hpp>
#include <sge/node.hpp>
#include <sge/camera.hpp>

SGE_BEGIN

class Scene {
public:
	Scene(void);
	virtual ~Scene(void);

public:
	bool init(void);
	void update(float elapsed);
	Node *rootNode(void);

protected:
	void updateNode(Node *node, float elapsed);

private:
	Node m_rootNode;
};

inline Node *Scene::rootNode(void)
{
	return &m_rootNode;
}

SGE_END

#endif // SGE_SCENE_HPP

