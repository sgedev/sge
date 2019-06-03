//
//
#ifndef SGE_SCENE_CAMERA_HPP
#define SGE_SCENE_CAMERA_HPP

#include <sge/scene/camera/common.hpp>

SGE_SCENE_CAMERA_BEGIN

bool init(void);
void shutdown(void);
void reset(void);
bool load(void);
void update(void);
void mouse_look(float dx, float dy);
float move_speed(void);
void set_move_speed(float v);
void move_forward(float elapsed);
void move_backward(float elapsed);
void move_left(float elapsed);
void move_right(float elapsed);
float fov(void);
void set_fov(float v);

SGE_SCENE_CAMERA_END

#endif // SGE_SCENE_CAMERA_HPP

