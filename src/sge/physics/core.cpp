/*
 *
 */
#include <lua.hpp>

#include <sge/physics.h>

extern "C" void sge_physics_init(lua_State *L)
{
	lua_newtable(L);

	lua_setfield(L, -2, "physics");
}

extern "C" void sge_physics_shutdown(void)
{
}

extern "C" bool sge_physics_start(void)
{
	return true;
}

extern "C" void sge_physics_stop(void)
{

}
