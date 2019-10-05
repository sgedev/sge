//
//
#include "project.hpp"

Project::Project(void)
{
}

Project::~Project(void)
{
}

bool Project::init(const char *root)
{
	if (!SGE::Game::init())
		return false;

	PHYSFS_makeCurrent(fs());
	PHYSFS_mount(root, "/", 0);

	return true;
}

void Project::shutdown(void)
{
	SGE::Game::shutdown();
}

bool Project::handleEvent(const SGE::Event *evt)
{
	return SGE::Game::handleEvent(evt);
}

void Project::update(float elapsed)
{
	SGE::Game::update(elapsed);
}

void Project::draw(SGE::View *v)
{
	SGE::Game::draw(v);
}

bool Project::save(const char *path)
{
	return true;
}

bool Project::load(const char *path)
{
	return true;
}

bool Project::importFile(const char *import_path, const char *filename)
{
	return true;
}

bool Project::exportFile(const char *export_path, const char *filename)
{
	return true;
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
