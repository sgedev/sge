//
//
#include <btBulletDynamicsCommon.h>

#include <sge/db.hpp>
#include <sge/scene.hpp>

SGE_SCENE_BEGIN

static btDefaultCollisionConfiguration *bt_cc;
static btCollisionDispatcher *bt_cd;
static btBroadphaseInterface *bt_bi;
static btSequentialImpulseConstraintSolver *bt_sics;
static btDiscreteDynamicsWorld *bt_world;

bool init(void)
{
	bt_cc = new btDefaultCollisionConfiguration();
	bt_cd = new btCollisionDispatcher (bt_cc);
	bt_bi = new btDbvtBroadphase();
	bt_sics = new btSequentialImpulseConstraintSolver();
	bt_world = new btDiscreteDynamicsWorld (bt_cd, bt_bi, bt_sics, bt_cc);

	renderer::init();
	camera::init();

	return true;
}

void shutdown(void)
{
	camera::shutdown();
	renderer::shutdown();
}

bool load(const char *path)
{
	if (path == NULL) {
		// TODO clear scene.
		return true;
	}

	db scene_db = db::root().select(path);
	db gravity_node = scene_db.select("/gravity");
	glm::vec3 gravity(
		gravity_node.select("/x").to_float(),
		gravity_node.select("/y").to_float(),
		gravity_node.select("/z").to_float());

	return true;
}

void update(void)
{
}

void draw(void)
{
	renderer::begin();

	camera::setup();

	renderer::end();
}

SGE_SCENE_END

