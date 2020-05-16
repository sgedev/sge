//
//
#include <SGE/Physics/World.hpp>

SGE_PHYSICS_BEGIN

World::World(void)
{
}

World::~World(void)
{
}

bool World::init(void)
{
	return true;
}

void World::objectAdded(ObjectPtr obj)
{
}

void World::objectRemoved(ObjectPtr obj)
{
}

void World::cameraAdded(CameraPtr cam)
{
}

void World::cameraRemoved(CameraPtr cam)
{
}

void World::update(float elapsed)
{
}

SGE_PHYSICS_END
