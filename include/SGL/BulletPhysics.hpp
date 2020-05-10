//
//
#ifndef SGL_BULLETPHYSICS_HPP
#define SGL_BULLETPHYSICS_HPP

#include <SGL/Common.hpp>
#include <SGL/Physics.hpp>

SGL_BEGIN

class BulletPhysics: public Physics {
public:
	BulletPhysics(void);
	virtual ~BulletPhysics(void);

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

#endif // SGL_BULLETPHYSICS_HPP

