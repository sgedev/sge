//
//
#ifndef SGE_PHYSICS_WORLD_HPP
#define SGE_PHYSICS_WORLD_HPP

#include <SGE/Scene.hpp>
#include <SGE/Physics/Common.hpp>

SGE_PHYSICS_BEGIN

class World {
public:
	World(void);
	virtual ~World(void);

public:
	bool init(Scene &scene);
	void shutdown(void);

protected:
	void objectAdded(ObjectPtr obj);
	void objectRemoved(ObjectPtr obj);

private:
	Scene *m_scene;
};

SGE_PHYSICS_END

#endif // SGE_PHYSICS_WORLD_HPP
