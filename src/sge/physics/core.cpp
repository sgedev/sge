/*
 *
 */
#include <lua.hpp>
#include <btBulletCollisionCommon.h>

#include <sge/physics.h>

extern "C" void sge_physics_export(lua_State *L)
{
	lua_newtable(L);

	lua_setfield(L, -2, "physics");
}

extern "C" bool sge_physics_init(void)
{
	return true;
}

extern "C" void sge_physics_shutdown(void)
{
}

extern "C" void sge_physics_update(float elapsed)
{
}
