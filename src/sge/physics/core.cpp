/*
 *
 */
#include <lua.hpp>
#include <btBulletDynamicsCommon.h>

#include <sge/object.h>
#include <sge/physics.h>

#define SGE_PHYSICS_TYPE "sge.physics"

static btDefaultCollisionConfiguration *sge_physics_cc;
static btCollisionDispatcher *sge_physics_cd;
static btBroadphaseInterface *sge_physics_bi;
static btSequentialImpulseConstraintSolver *sge_physics_sics;
static btDiscreteDynamicsWorld *sge_physics_world;
static btVector3 sge_physics_gravity(0.0f, -9.8f, 0.0f);

static void sge_physics_object_added(sge_object_t *object)
{

}

static void sge_physics_object_removed(sge_object_t *object)
{

}

static int sge_physics_index(lua_State *L)
{
	return 0;
}

static int sge_physics_newindex(lua_State *L)
{
	return 0;
}

extern "C" void sge_physics_export(lua_State *L)
{
	/* matetable */

	luaL_newmetatable(L, SGE_PHYSICS_TYPE);

	lua_pushcfunction(L, &sge_physics_index);
	lua_setfield(L, -2, "__index");

	lua_pushcfunction(L, &sge_physics_index);
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

	return true;
}

extern "C" void sge_physics_shutdown(void)
{
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
