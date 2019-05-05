//
//

#include <sge/scene/base.hpp>

SGE_SCENE_BEGIN

bool base::init(void)
{
	SGE_LOGI("Bullet: %d\n", btGetVersion());

	s_bt_cc = new btDefaultCollisionConfiguration();
	s_bt_cd = new btCollisionDispatcher(s_bt_cc);
	s_bt_bi = new btDbvtBroadphase();
	s_bt_sics = new btSequentialImpulseConstraintSolver();
	s_bt_world = new btDiscreteDynamicsWorld(s_bt_cd, s_bt_bi, s_bt_sics, s_bt_cc);

	s_physics_enabled = false;

	return true;
}

void base::shutdown(void)
{
	delete s_bt_world;
	delete s_bt_sics;
	delete s_bt_bi;
	delete s_bt_cd;
	delete s_bt_cc;
}



void base::update(float elspaed)
{
	update_physics();

	//if (s_state != STATE_READY)
	//	return;
}

void base::draw(camera &cam)
{
	//if (s_state != STATE_READY)
	//	return;
}

void base::reset(void)
{
}

void base::update_physics(float elapsed)
{
	if (!s_physics_enabled)
		return;

	s_bt_world->stepSimulation(elapsed);

	for (int i = s_bt_world->getNumCollisionObjects() - 1; i >= 0; --i) {
	}
}

SGE_SCENE_END

