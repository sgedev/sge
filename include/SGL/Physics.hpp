//
//
#ifndef SGL_PHYSICS_HPP
#define SGL_PHYSICS_HPP

#include <SGL/Common.hpp>
#include <SGL/Scene.hpp>

SGL_BEGIN

class Physics: public Scene::Listener {
public:
	Physics(void);
	virtual ~Physics(void);

public:
	bool init(void);
	void objectAdded(ObjectPtr obj) override;
	void objectRemoved(ObjectPtr obj) override;
	void cameraAdded(CameraPtr cam) override;
	void cameraRemoved(CameraPtr cam) override;
	void update(float elapsed) override;
	void draw(void) override;

private:
};

SGL_END

#endif // SGL_PHYSICS_HPP

