//
//
#include <SGE/Physics/World.hpp>

SGE_PHYSICS_BEGIN

World::World(void):
	m_scene(NULL)
{
}

World::~World(void)
{
	if (m_scene != NULL)
		shutdown();
}

bool World::init(Scene &scene)
{
	SGE_ASSERT(m_scene == NULL);

	m_scene = &scene;

	m_scene->addObject.connect<&World::objectAdded>(this);
	m_scene->removeObject.connect<&World::objectRemoved>(this);

	return true;
}

void World::shutdown(void)
{
	SGE_ASSERT(m_scene != NULL);
	m_scene = NULL;
}

void World::objectAdded(ObjectPtr obj)
{

}

void World::objectRemoved(ObjectPtr obj)
{

}

SGE_PHYSICS_END
