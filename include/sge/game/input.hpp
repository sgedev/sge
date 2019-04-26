//
//
#ifndef SGE_GAME_INPUT_HPP
#define SGE_GAME_INPUT_HPP

#include <sge/game/common.hpp>

#define SGE_GAME_INPUT_BEGIN SGE_GAME_BEGIN namespace input {
#define SGE_GAME_INPUT_END } SGE_GAME_END

SGE_GAME_INPUT_BEGIN

typedef void (* key_handler)(SDL_Keycode key, void *data);
typedef void (* mouse_motion_handler)(float xoffset, float yoffset, void *data);

bool init(void);
void shutdown(void);
void update(void);
void handle_event(const SDL_Event &event);
void bind(SDL_Keycode key, key_handler handler, void *data);
void unbind(SDL_Keycode key);
void unbindall(void);
void set_mouse_motion_handler(mouse_motion_handler handler, void *data);

SGE_GAME_INPUT_END

#endif // SGE_GAME_INPUT_HPP

