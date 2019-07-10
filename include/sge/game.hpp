//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <sge/game/common.hpp>

SGE_GAME_BEGIN

bool init(void);
void shutdown(void);
bool handle_event(const SDL_Event &event);
void frame(float elapsed);
bool is_running(void);

SGE_GAME_END

#endif // SGE_GAME_HPP

