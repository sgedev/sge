//
//
#ifndef SGE_SCENE_CAMERA_HPP
#define SGE_SCENE_CAMERA_HPP

#include <sge/db.hpp>
#include <sge/scene/camera/common.hpp>

SGE_SCENE_CAMERA_BEGIN

bool init(void);
void shutdown(void);
void reset(void);
bool load(db::node node);
void update(void);
void mouse_look(float dx, float dy);
float move_speed(void);
void set_move_speed(float v);
void move_forward(void);
void move_backward(void);
void move_left(void);
void move_right(void);
float fov(void);
void set_fov(float v);

SGE_SCENE_CAMERA_END

#endif // SGE_SCENE_CAMERA_HPP

