//
//
#ifndef SGE_SCENE_CAMERA_HPP
#define SGE_SCENE_CAMERA_HPP

#include <sge/scene/common.hpp>

#define SGE_SCENE_CAMERA_BEGIN SGE_SCENE_BEGIN namespace camera {
#define SGE_SCENE_CAMERA_END } SGE_SCENE_END

SGE_SCENE_CAMERA_BEGIN

bool init(void);
void shutdown(void);
void setup(void);

SGE_SCENE_CAMERA_END

#endif // SGE_SCENE_CAMERA_HPP

