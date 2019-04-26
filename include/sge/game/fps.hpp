//
//
#ifndef SGE_GAME_FPS_HPP
#define SGE_GAME_FPS_HPP

#include <sge/game/common.hpp>

#define SGE_GAME_FPS_BEGIN SGE_GAME_BEGIN namespace fps {
#define SGE_GAME_FPS_END } SGE_GAME_END

SGE_GAME_FPS_BEGIN

bool init(void);
void shutdown(void);
void update(void);
void set_mouse_sensitivity(float v);
void mouse_look(float dx, float dy);
void set_move_speed(float v);
void move_forward(void);
void move_backward(void);
void move_left(void);
void move_right(void);
void set_fov(float v);

SGE_GAME_FPS_END

#endif // SGE_GAME_HELPER_FPS_HPP

