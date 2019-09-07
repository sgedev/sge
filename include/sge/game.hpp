//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <functional>

#include <sge/common.hpp>

#define SGE_GAME_BEGIN SGE_BEGIN namespace game {
#define SGE_GAME_END } SGE_END

SGE_GAME_BEGIN

enum state {
	STATE_IDLE = 0,
	STATE_LOADING,
	STATE_READY,
	STATE_PLAYING,
};

typedef unsigned int (*trap_fps_func)(void);

extern trap_fps_func trap_fps;

bool init(void);
void shutdown(void);
void handle_event(const SDL_Event *event);
void update(float elapsed);
void draw(void);
state current_state(void);

SGE_GAME_END

#endif // SGE_VM_HPP

