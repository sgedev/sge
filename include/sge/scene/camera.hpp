//
//
#ifndef SGE_SCENE_CAMERA_HPP
#define SGE_SCENE_CAMERA_HPP

#include <sge/scene/camera/common.hpp>

SGE_SCENE_CAMERA_BEGIN

bool init(void);
void shutdown(void);
void clear(void);
void render(void);
void set_transform(const glm::mat4 &v);
void set_projection(const glm::mat4 &v);

SGE_SCENE_CAMERA_END

#endif // SGE_SCENE_CAMERA_HPP

