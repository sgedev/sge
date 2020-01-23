//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <sge/common.hpp>

#define SGE_GAME_BEGIN SGE_BEGIN namespace Game {
#define SGE_GAME_END } SGE_END

SGE_GAME_BEGIN

struct TrapHandler {
	int (*fps)(void);
};

bool init(void);
void shutdown(void);
bool handleEvent(const SDL_Event &event);
void update(float elapsed, const TrapHandler *th);

SGE_GAME_END

#endif // SGE_GAME_HPP
