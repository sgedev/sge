//
//
#include <btBulletDynamicsCommon.h>

#include <sge/db.hpp>
#include <sge/scene.hpp>

SGE_SCENE_BEGIN

enum state {
	STATE_IDLE = 0,
	STATE_LOADING,
	STATE_READY,
	STATE_CANCELING
};

static db::node s_node;
static state s_state;
static bool s_physics_enabled;
static btDefaultCollisionConfiguration *s_bt_cc;
static btCollisionDispatcher *s_bt_cd;
static btBroadphaseInterface *s_bt_bi;
static btSequentialImpulseConstraintSolver *s_bt_sics;
static btDiscreteDynamicsWorld *s_bt_world;

static void update_physics(float elapsed)
{
	if (!s_physics_enabled)
		return;

	s_bt_world->stepSimulation(elapsed);

	for (int i = s_bt_world->getNumCollisionObjects() - 1; i >= 0; --i) {
	}
}

namespace loading {
	static uv_work_t s_work;
	static uv_async_t s_progress;
	static int s_percentage;

	static bool progress_set(int percentage)
	{
		if (s_state == STATE_CANCELING)
			return false;

		s_progress.data = (void *)(long)percentage;
		uv_async_send(&s_progress);

		return true;
	}

	static void work(uv_work_t *req)
	{
		if (!s_node) {
			s_progress.data = (void *)100;
			uv_async_send(&s_progress);
			return;
		}

		db::node physics_node = s_node.child("/physics");

		s_physics_enabled = (physics_node && physics_node.to_bool());
		if (s_physics_enabled) {
			db::node gravity_node = physics_node.child("/gravity");
			if (gravity_node) {
				s_bt_world->setGravity(btVector3(
					gravity_node.child("/x").to_float(),
					gravity_node.child("/y").to_float(),
					gravity_node.child("/z").to_float()));
			}
		}

		if (!progress_set(1))
			return;

		db::node camera_node = s_node.child("/camera");
		if (!camera_node) {
			// TODO abort.
			return;
		}

		if (!camera::load(camera_node)) {
			// TODO abort.
			return;
		}

		// load world.
		// load entities.
	}

	static void work_done(uv_work_t *req, int status)
	{
		SGE_ASSERT(s_state == STATE_LOADING);

		if (status) {
			s_state = STATE_IDLE;
			return;
		}

		s_state = STATE_READY;
	}

	static void progress(uv_async_t *handle)
	{
		SGE_ASSERT(s_state == STATE_LOADING);

		s_percentage = (int)(long)(handle->data);
	}

	static void init(void)
	{
		uv_async_init(uv_default_loop(), &s_progress, progress);
	}

	bool start(db::node node)
	{
		if (s_state != STATE_IDLE)
			return false;

		s_node = node;
		s_state = STATE_LOADING;
		s_progress.data = (void *)0;
		s_percentage = 0;

		uv_queue_work(uv_default_loop(), &s_work, work, work_done);

		return true;
	}

	bool is_started(void)
	{
		return (s_state == STATE_LOADING);
	}

	void stop(void)
	{
		if (s_state != STATE_LOADING)
			return;

		s_state = STATE_CANCELING;

		// TODO
	}

	int percentage(void)
	{
		if (s_state != STATE_LOADING)
			return -1;

		return s_percentage;
	}
}

bool init(void)
{
	SGE_LOGI("Bullet: %d\n", btGetVersion());

	s_bt_cc = new btDefaultCollisionConfiguration();
	s_bt_cd = new btCollisionDispatcher(s_bt_cc);
	s_bt_bi = new btDbvtBroadphase();
	s_bt_sics = new btSequentialImpulseConstraintSolver();
	s_bt_world = new btDiscreteDynamicsWorld(s_bt_cd, s_bt_bi, s_bt_sics, s_bt_cc);

	camera::init();
	loading::init();

	s_physics_enabled = false;
	s_state = STATE_IDLE;

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
	update_physics(elapsed);

	//if (s_state != STATE_READY)
	//	return;
}

void draw(void)
{
	//if (s_state != STATE_READY)
	//	return;

	camera::update();
}

void reset(void)
{
	switch (s_state) {
	case STATE_IDLE:
		break;
	case STATE_LOADING:
		break;
	case STATE_READY:
		break;
	}
}

SGE_SCENE_END

