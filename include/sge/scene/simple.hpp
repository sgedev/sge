//
//
#ifndef SGE_SCENE_SIMPLE_HPP
#define SGE_SCENE_SIMPLE_HPP

#include <sge/scene/common.hpp>
#include <sge/scene/base.hpp>

SGE_SCENE_BEGIN

class simple: public base {
public:
	simple(void);
	virtual ~simple(void);

public:
	virtual bool init(void);
	virtual void shutdown(void);
	virtual void update(float elapsed);
	virtual void draw(camera *cam);

private:
	camera *m_camera;
};

SGE_SCENE_END

#endif // SGE_SCENE_BASE_HPP

