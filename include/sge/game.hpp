//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <sge/game/common.hpp>
#include <sge/game/input.hpp>
#include <sge/game/fps.hpp>

SGE_GAME_BEGIN

bool init(void);
void shutdown(void);
void update(void);
bool handle_event(const SDL_Event &event);

SGE_GAME_END

#endif // SGE_GAME_HPP
