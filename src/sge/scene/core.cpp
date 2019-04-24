//
//
#include <btBulletDynamicsCommon.h>

#include <sge/scene.hpp>

SGE_SCENE_BEGIN

static btDefaultCollisionConfiguration *s_bt_cc;
static btCollisionDispatcher *s_bt_cd;
static btBroadphaseInterface *s_bt_bi;
static btSequentialImpulseConstraintSolver *s_bt_sics;
static btDiscreteDynamicsWorld *s_bt_world;

bool init(void)
{
	s_bt_cc = new btDefaultCollisionConfiguration();
	s_bt_cd = new btCollisionDispatcher(s_bt_cc);
	s_bt_bi = new btDbvtBroadphase();
	s_bt_sics = new btSequentialImpulseConstraintSolver;
	s_bt_world = new btDiscreteDynamicsWorld (s_bt_cd, s_bt_bi, s_bt_sics, s_bt_cc);

	s_bt_world->setGravity(btVector3 (0, -10 ,0));

	camera::init();

	return true;
}

void shutdown(void)
{
	camera::shutdown();

	delete s_bt_world;
	delete s_bt_sics;
	delete s_bt_bi;
	delete s_bt_cd;
	delete s_bt_cc;
}

void update(float elapsed)
{
	s_bt_world->stepSimulation(elapsed, 10);

	for (int i = s_bt_world->getNumCollisionObjects() - 1; i >= 0; --i) {
	}

	camera::reset();

	// drawing...
}

void draw(void)
{
	camera::render();
}

SGE_SCENE_END

