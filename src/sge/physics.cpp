//
//
#include <btBulletDynamicsCommon.h>

#include <sge/physics.h>

static btDefaultCollisionConfiguration *sge_physics_cc;
static btCollisionDispatcher *sge_physics_cd;
static btBroadphaseInterface *sge_physics_bi;
static btSequentialImpulseConstraintSolver *sge_physics_sics;
static btDiscreteDynamicsWorld *sge_physics_world;

int sge_physics_init(void)
{
	sge_physics_cc = new btDefaultCollisionConfiguration();
	if (sge_physics_cc == NULL)
		goto bad0;

	sge_physics_cd = new btCollisionDispatcher(sge_physics_cc);
	if (sge_physics_cd == NULL)
		goto bad1;

	sge_physics_bi = new btDbvtBroadphase();
	if (sge_physics_bi == NULL)
		goto bad2;

	sge_physics_sics = new btSequentialImpulseConstraintSolver();
	if (sge_physics_sics == NULL)
		goto bad3;

	sge_physics_world = new btDiscreteDynamicsWorld(
		sge_physics_cd,
		sge_physics_bi,
		sge_physics_sics,
		sge_physics_cc);

	if (sge_physics_world == NULL)
		goto bad4;

	sge_physics_world->setGravity(btVector3(0, -9.8f, 0));

	return 0;

bad4:
	delete sge_physics_sics;

bad3:
	delete sge_physics_bi;

bad2:
	delete sge_physics_cd;

bad1:
	delete sge_physics_cc;

bad0:
	return -1;
}

void sge_physics_shutdown(void)
{
	delete sge_physics_world;
	delete sge_physics_sics;
	delete sge_physics_bi;
	delete sge_physics_cd;
	delete sge_physics_cc;
}

extern "C" void sge_physics_update(float elapsed)
{
	if (sge_physics_world != NULL) {
		sge_physics_world->stepSimulation(elapsed);
		/* TODO */
	}
}

