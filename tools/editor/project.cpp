//
//
#include "project.hpp"

Project::Project(void)
{
}

Project::~Project(void)
{
}

void Project::update(void)
{
}

int Project::trapFps(lua_State *L)
{
	lua_pushinteger(L, 0);
	return 1;
}

int Project::trapEditorIsEnabled(lua_State *L)
{
	lua_pushboolean(L, 1);
	return 1;
}
