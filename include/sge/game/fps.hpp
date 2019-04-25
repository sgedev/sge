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
void update(float elapsed);
void handle_event(const SDL_Event &event);

SGE_GAME_FPS_END

#endif // SGE_GAME_HELPER_FPS_HPP

