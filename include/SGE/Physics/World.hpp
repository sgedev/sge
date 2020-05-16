//
//
#ifndef SGE_PHYSICS_WORLD_HPP
#define SGE_PHYSICS_WORLD_HPP

#include <SGE/Scene.hpp>
#include <SGE/Physics/Common.hpp>

SGE_PHYSICS_BEGIN

class World: public Scene::Listener {
public:
	World(void);
	virtual ~World(void);

public:
	bool init(void);
	void objectAdded(ObjectPtr obj) override;
	void objectRemoved(ObjectPtr obj) override;
	void cameraAdded(CameraPtr cam) override;
	void cameraRemoved(CameraPtr cam) override;
	void update(float elapsed) override;

private:
};

SGE_PHYSICS_END

#endif // SGE_PHYSICS_WORLD_HPP
