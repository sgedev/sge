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

protected:
	void objectAdded(ObjectPtr obj);
	void objectRemoved(ObjectPtr obj);
	void cameraAdded(CameraPtr cam);
	void cameraRemoved(CameraPtr cam);
	void update(float elapsed);
	void draw(void);

private:
};

SGL_END

#endif // SGL_PHYSICS_HPP

