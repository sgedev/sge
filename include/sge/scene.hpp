//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <QObject>

#include <sge/common.hpp>
#include <sge/node.hpp>
#include <sge/camera.hpp>
#include <sge/world.hpp>
#include <sge/entity.hpp>

SGE_BEGIN

class Scene: public QObject {
	Q_OBJECT

public:
	Scene(QObject *parent = Q_NULLPTR);
	virtual ~Scene(void);

signals:
	void nodeAdded(Node *node);
	void nodeRemoved(Node *node);

public:
	bool init(void);
	void update(float elapsed);
	Node *rootNode(void);
	void buildView(View &view, Camera *from) const;

protected:
	void buildNodeView(const Node *node, View &view, Camera *from) const;

protected slots:
	void childAdded(Node *child);
	void childRemoved(Node *child);

private:
	Node m_rootNode;
};

inline Node *Scene::rootNode(void)
{
	return &m_rootNode;
}

SGE_END

#endif // SGE_SCENE_HPP

