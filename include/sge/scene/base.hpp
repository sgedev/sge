//
//
#ifndef SGE_SCENE_BASE_HPP
#define SGE_SCENE_BASE_HPP

#include <sge/scene/common.hpp>
#include <sge/scene/camera.hpp>

SGE_SCENE_BEGIN

class base {
public:
	base(void);
	virtual ~base(void);

public:
	virtual bool init(void) = 0;
	virtual void shutdown(void) = 0;
	virtual void update(float elapsed) = 0;
	virtual void draw(camera *cam) = 0;
};

SGE_SCENE_END

#endif // SGE_SCENE_BASE_HPP

