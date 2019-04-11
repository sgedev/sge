//
//
#ifndef SGE_SCENE_SYSTEM_HPP
#define SGE_SCENE_SYSTEM_HPP

#include <btBulletDynamicsCommon.h>

#include <sge/scene/common.hpp>
#include <sge/scene/renderer.hpp>
#include <sge/scene/camera.hpp>

SGE_SCENE_BEGIN

class system {
public:
	system(void);
	virtual ~system(void);

public:
	bool init(void);
	void shutdown(void);
	void update(float elapsed);
	void draw(void);

private:
	renderer m_renderer;
	camera m_camera;

private:
	btDefaultCollisionConfiguration *m_cc;
	btCollisionDispatcher *m_cd;
	btBroadphaseInterface *m_bi;
	btSequentialImpulseConstraintSolver *m_sics;
	btDiscreteDynamicsWorld *m_dynamics_world;
};

SGE_SCENE_END

#endif // SGE_SCENE_SYSTEM_HPP

