//
//
#include <sge/physics.hpp>

SGE_BEGIN

Physics::Physics(QObject *parent)
	: QObject(parent)
	, m_btCC(Q_NULLPTR)
	, m_btCD(Q_NULLPTR)
	, m_btBI(Q_NULLPTR)
	, m_btSIC(Q_NULLPTR)
	, m_btWorld(Q_NULLPTR)
{
}

Physics::~Physics(void)
{
	if (m_btWorld != Q_NULLPTR)
		delete m_btWorld;

	if (m_btSIC != Q_NULLPTR)
		delete m_btSIC;

	if (m_btBI != Q_NULLPTR)
		delete m_btBI;

	if (m_btCD != Q_NULLPTR)
		delete m_btCD;

	if (m_btCC != Q_NULLPTR)
		delete m_btCC;
}

bool Physics::init(Scene *scene)
{
	Q_ASSERT(scene != Q_NULLPTR);
	Q_ASSERT(m_btCC == Q_NULLPTR);
	Q_ASSERT(m_btCD == Q_NULLPTR);
	Q_ASSERT(m_btBI == Q_NULLPTR);
	Q_ASSERT(m_btSIC == Q_NULLPTR);
	Q_ASSERT(m_btWorld == Q_NULLPTR);

	m_btCC = new btDefaultCollisionConfiguration();
	if (m_btCC == Q_NULLPTR)
		goto bad0;

	m_btCD = new btCollisionDispatcher(m_btCC);
	if (m_btCD == Q_NULLPTR)
		goto bad1;

	m_btBI = new btDbvtBroadphase();
	if (m_btBI == Q_NULLPTR)
		goto bad2;

	m_btSIC = new btSequentialImpulseConstraintSolver();
	if (m_btSIC == Q_NULLPTR)
		goto bad3;

	m_btWorld = new btDiscreteDynamicsWorld(m_btCD, m_btBI, m_btSIC, m_btCC);
	if (m_btWorld == Q_NULLPTR)
		goto bad4;

	m_btWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));

	connect(scene, &Scene::nodeAdded, this, &Physics::nodeAdded);
	connect(scene, &Scene::nodeRemoved, this, &Physics::nodeRemoved);

	return true;

bad4:
	delete m_btSIC;
	m_btSIC = Q_NULLPTR;

bad3:
	delete m_btBI;
	m_btBI = Q_NULLPTR;

bad2:
	delete m_btCD;
	m_btCD = Q_NULLPTR;

bad1:
	delete m_btCC;
	m_btCC = Q_NULLPTR;

bad0:
	return false;
}

void Physics::update(float elapsed)
{
	Q_ASSERT(m_btWorld != Q_NULLPTR);

	m_btWorld->stepSimulation(elapsed);

	for (int j = m_btWorld->getNumCollisionObjects() - 1; j >= 0; --j) {
		btTransform trans;
		btCollisionObject *obj = m_btWorld->getCollisionObjectArray()[j];
		btRigidBody *body = btRigidBody::upcast(obj);
		if (body != Q_NULLPTR && body->getMotionState() != Q_NULLPTR)
			body->getMotionState()->getWorldTransform(trans);
		else
			trans = obj->getWorldTransform();
	}
}

void Physics::nodeAdded(Node *node)
{
}

void Physics::nodeRemoved(Node *node)
{
}

SGE_END

