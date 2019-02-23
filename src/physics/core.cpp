//
//
#include <btBulletDynamicsCommon.h>

#include <sge/physics.h>

SGE_PHYSICS_BEGIN

static btAxisSweep3 *broad_phase;
static btDefaultCollisionConfiguration *conf;
static btCollisionDispatcher *disp;
static btSequentialImpulseConstraintSolver *solver;
static btDiscreteDynamicsWorld *world;

bool Init(void)
{
	SGE_LOGI("Initializing physics...\n");

	btVector3 world_aabb_min(-10000, -10000, -10000);
	btVector3 world_aabb_max(10000, 10000, 10000);

	broad_phase = new btAxisSweep3(world_aabb_min, world_aabb_max);
	conf = new btDefaultCollisionConfiguration();
	disp = new btCollisionDispatcher(conf);
	solver = new btSequentialImpulseConstraintSolver;
	world = new btDiscreteDynamicsWorld(disp, broad_phase, solver, conf);

	return true;
}

void Shutdown(void)
{
	SGE_LOGI("Shuting down physics...\n");

	delete world;
	delete solver;
	delete disp;
	delete conf;
	delete broad_phase;
}

void Update(void)
{
	world->stepSimulation(GetElapsed());

	//btTransform trans;
	//fallRigidBody->getMotionState()->getworldTransform(trans);
}

void HandleEvent(const SDL_Event *event)
{
}

SGE_PHYSICS_END
