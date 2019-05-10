//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <sge/game/common.hpp>

SGE_GAME_BEGIN

typedef struct {
	struct {
		void (*set_fov)(float fov);
		void (*move_left)(int steps);
		void (*move_right)(int steps);
		void (*move_forward)(int steps);
	} camera;
} traps_t;

bool init(void);
void shutdown(void);
void update(float elapsed);
bool can_quit(void);

SGE_GAME_END

#endif // SGE_GAME_HPP

