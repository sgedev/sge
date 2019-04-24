//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <sge/scene/common.hpp>
#include <sge/scene/camera.hpp>

SGE_SCENE_BEGIN

bool init(void);
void shutdown(void);
void update(float elapsed);
void draw(void);

SGE_SCENE_END

#endif // SGE_SCENE_HPP

