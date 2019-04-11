//
//
#ifndef SGE_SCENE_CAMERA_HPP
#define SGE_SCENE_CAMERA_HPP

#include <sge/scene/common.hpp>
#include <sge/scene/renderer.hpp>

SGE_SCENE_BEGIN

class camera {
public:
	camera(void);
	virtual ~camera(void);

public:
	bool init(renderer *r);
	void shutdown(void);
	void setup(void);

private:
	renderer *m_renderer;
};

SGE_SCENE_END

#endif // SGE_SCENE_CAMERA_HPP

