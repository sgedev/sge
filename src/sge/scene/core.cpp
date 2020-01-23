//
//
#include <memory>

#include <btBulletDynamicsCommon.h>

#include <sge/scene.hpp>
#include <sge/renderer.hpp>

SGE_SCENE_BEGIN

static btDefaultCollisionConfiguration *collisionConfiguration;
static btCollisionDispatcher *dispatcher;
static btBroadphaseInterface *overlappingPairCache;
static btSequentialImpulseConstraintSolver *solver;
static btDiscreteDynamicsWorld *dynamicsWorld;

bool init(void)
{
	logInfo("BulletPhysics version: %d\n", btGetVersion());

	collisionConfiguration = new btDefaultCollisionConfiguration();
	if (collisionConfiguration == NULL)
		goto bad0;

	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	if (dispatcher == NULL)
		goto bad1;

	overlappingPairCache = new btDbvtBroadphase();
	if (overlappingPairCache == NULL)
		goto bad2;

	solver = new btSequentialImpulseConstraintSolver;
	if (solver == NULL)
		goto bad3;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	if (dynamicsWorld == NULL)
		goto bad4;

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	return true;

bad4:
	delete solver;
	solver = NULL;

bad3:
	delete overlappingPairCache;
	overlappingPairCache = NULL;

bad2:
	delete dispatcher;
	dispatcher = NULL;

bad1:
	delete collisionConfiguration;
	collisionConfiguration = NULL;

bad0:
	return false;
}

void shutdown(void)
{
	SGE_ASSERT(dynamicsWorld != NULL);
	delete dynamicsWorld;
	dynamicsWorld = NULL;

	SGE_ASSERT(solver != NULL);
	delete solver;
	solver = NULL;

	SGE_ASSERT(overlappingPairCache != NULL);
	delete overlappingPairCache;
	overlappingPairCache = NULL;

	SGE_ASSERT(dispatcher != NULL);
	delete dispatcher;
	dispatcher = NULL;

	SGE_ASSERT(collisionConfiguration != NULL);
	delete collisionConfiguration;
	collisionConfiguration = NULL;
}

void update(float elapsed)
{
	SGE_ASSERT(dynamicsWorld != NULL);

	dynamicsWorld->stepSimulation(elapsed);
}

void draw(void)
{
}

SGE_SCENE_END
