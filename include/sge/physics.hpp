//
//
#ifndef SGE_PHYSICS_HPP
#define SGE_PHYSICS_HPP

#include <QObject>

#include <btBulletDynamicsCommon.h>

#include <sge/common.hpp>
#include <sge/scene.hpp>

SGE_BEGIN

class Physics: public QObject {
	Q_OBJECT

public:
	Physics(QObject *parent = Q_NULLPTR);
	virtual ~Physics(void);

public:
	bool init(Scene *scene);
	void update(float elapsed);

protected slots:
	void nodeAdded(Node *node);
	void nodeRemoved(Node *node);

private:
	btDefaultCollisionConfiguration *m_btCC;
	btCollisionDispatcher *m_btCD;
	btBroadphaseInterface *m_btBI;
	btSequentialImpulseConstraintSolver *m_btSIC;
	btDiscreteDynamicsWorld *m_btWorld;
};

SGE_END

#endif // SGE_PHYSICS_HPP

