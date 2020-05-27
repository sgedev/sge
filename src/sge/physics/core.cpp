/*
 *
 */
#include <lua.hpp>
#include <btBulletDynamicsCommon.h>

#include <sge/object.h>
#include <sge/physics.h>

#define SGE_PHYSICS_TYPE "sge.physics"

typedef struct {
	sge_object_t *obj;
	sge_object_watch_t *watch;
} sge_physics_object_t;

static btDefaultCollisionConfiguration *sge_physics_cc;
static btCollisionDispatcher *sge_physics_cd;
static btBroadphaseInterface *sge_physics_bi;
static btSequentialImpulseConstraintSolver *sge_physics_sics;
static btDiscreteDynamicsWorld *sge_physics_world;
static btVector3 sge_physics_gravity(0.0f, -9.8f, 0.0f);
static sge_object_new_watch_t *sge_physics_object_new_watch;

static void sge_physics_object_destroy(sge_object_t *obj, void *data)
{
	sge_physics_object_t *pobj = (sge_physics_object_t *)data;

	sge_object_remove_watch(obj, pobj->watch);
	free(pobj);
}

static void sge_physics_object_set_enabled(sge_object_t *obj, bool v, void *data)
{
	sge_physics_object_t *pobj = (sge_physics_object_t *)data;
}

static void sge_physics_object_set_visibled(sge_object_t *obj, bool v, void *data)
{
	sge_physics_object_t *pobj = (sge_physics_object_t *)data;
}

static void sge_physics_object_set_pos(sge_object_t *obj, const cx_vec3_t *p, void *data)
{
	sge_physics_object_t *pobj = (sge_physics_object_t *)data;
}

static void sge_physics_object_set_scale(sge_object_t *obj, const cx_vec3_t *p, void *data)
{
	sge_physics_object_t *pobj = (sge_physics_object_t *)data;
}

static void sge_physics_object_set_rotation(sge_object_t *obj, const cx_quat_t *p, void *data)
{
	sge_physics_object_t *pobj = (sge_physics_object_t *)data;
}

static void sge_physics_object_set_mass(sge_object_t *obj, float v, void *data)
{
	sge_physics_object_t *pobj = (sge_physics_object_t *)data;
}

static void sge_physics_object_set_property(sge_object_t *obj, const char *prop, const char *value, void *data)
{
	sge_physics_object_t *pobj = (sge_physics_object_t *)data;
}

static void sge_physics_object_new(sge_object_t *obj, void *data)
{
	static const sge_object_watch_funcs_t watch_funcs = {
		.destroy = sge_physics_object_destroy,
		.set_enabled = sge_physics_object_set_enabled,
		.set_pos = sge_physics_object_set_pos,
		.set_scale = sge_physics_object_set_scale,
		.set_rotation = sge_physics_object_set_rotation,
		.set_mass = sge_physics_object_set_mass,
		.set_property = sge_physics_object_set_property
	};

	sge_physics_object_t *pobj = (sge_physics_object_t *)malloc(sizeof(sge_physics_object_t));
	if (pobj == NULL) {
		// TODO
		return;
	}

	pobj->obj = obj;
	pobj->watch = sge_object_add_watch(obj, &watch_funcs, pobj);
}

static int sge_physics_newindex(lua_State *L)
{
	return 0;
}

extern "C" void sge_physics_export(lua_State *L)
{
	luaL_newmetatable(L, SGE_PHYSICS_TYPE);

	lua_pushcfunction(L, &sge_physics_newindex);
	lua_setfield(L, -2, "__newindex");

	lua_pop(L, 1);

	lua_newtable(L);
	luaL_setmetatable(L, SGE_PHYSICS_TYPE);
	lua_setfield(L, -2, "physics");
}

extern "C" bool sge_physics_init(void)
{
	sge_physics_cc = new btDefaultCollisionConfiguration();
	sge_physics_cd = new btCollisionDispatcher(sge_physics_cc);
	sge_physics_bi = new btDbvtBroadphase();
	sge_physics_sics = new btSequentialImpulseConstraintSolver;
	sge_physics_world = new btDiscreteDynamicsWorld(sge_physics_cd, sge_physics_bi, sge_physics_sics, sge_physics_cc);

	sge_physics_world->setGravity(sge_physics_gravity);

	SGE_ASSERT(sge_physics_object_new_watch == NULL);
	sge_physics_object_new_watch = sge_object_add_new_watch(sge_physics_object_new, NULL);

	return true;
}

extern "C" void sge_physics_shutdown(void)
{
	SGE_ASSERT(sge_physics_object_new_watch != NULL);
	sge_object_remove_new_watch(sge_physics_object_new_watch);
	sge_physics_object_new_watch = NULL;
}

extern "C" void sge_physics_update(float elapsed)
{
	sge_physics_world->stepSimulation(elapsed);

	for (int i = sge_physics_world->getNumCollisionObjects() - 1; i >= 0; --i) {
		btCollisionObject *obj = sge_physics_world->getCollisionObjectArray()[i];
		btRigidBody *body = btRigidBody::upcast(obj) ;
		btTransform trans;
		if (body && body->getMotionState())
			body->getMotionState()->getWorldTransform(trans) ;
		else
			trans = obj->getWorldTransform();
		SGE_LOGI("world pos object %d = %f, %f, %f\n", i,
			float(trans.getOrigin().getX()),
			float(trans.getOrigin().getY()),
			float(trans.getOrigin().getZ()));
	}
}
