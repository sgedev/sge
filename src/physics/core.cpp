//
//
#include "SGL_physics.h"

SGL_PHYSICS_BEGIN

static btAxisSweep3 *BroadPhase;
static btDefaultCollisionConfiguration *Configuration;
static btCollisionDispatcher *Dispatcher;
static btSequentialImpulseConstraintSolver *Solver;
static btDiscreteDynamicsWorld *World;

bool Init(void)
{
	btVector3 worldAabbMin(-10000, -10000, -10000);
	btVector3 worldAabbMax(10000, 10000, 10000);

	BroadPhase = new btAxisSweep3(worldAabbMin, worldAabbMax);
	Configuration = new btDefaultCollisionConfiguration();
	Dispatcher = new btCollisionDispatcher(Configuration);
	Solver = new btSequentialImpulseConstraintSolver;
	World = new btDiscreteDynamicsWorld(Dispatcher, BroadPhase, Solver, Configuration);

	return true;
}

void Shutdown(void)
{
	delete World;
	delete Solver;
	delete Dispatcher;
	delete Configuration;
	delete BroadPhase;
}

void Update(float elapsed)
{
	World->stepSimulation(elapsed);

	//btTransform trans;
	//fallRigidBody->getMotionState()->getWorldTransform(trans);
}

void HandleEvent(const SDL_Event *event)
{
}

void AddObject(Object *obj)
{
	SDL_assert(obj != NULL);
}

void RemoveObject(Object *obj)
{
	SDL_assert(obj != NULL);
}

SGL_PHYSICS_END
