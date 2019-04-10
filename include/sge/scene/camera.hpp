//
//
#ifndef SGE_SCENE_CAMERA_HPP
#define SGE_SCENE_CAMERA_HPP

#include <sge/renderer.hpp>
#include <sge/scene/common.hpp>

SGE_SCENE_BEGIN

class camera {
public:
	camera(void);
	virtual ~camera(void);

public:
	bool init(void);
	void shutdown(void);
	void draw(void);

private:
	renderer::view m_view;
};

SGE_SCENE_END

#endif // SGE_SCENE_CAMERA_HPP

