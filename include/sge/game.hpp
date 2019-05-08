//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <sge/game/common.hpp>
#include <sge/game/vm.hpp>

SGE_GAME_BEGIN

bool init(void);
void shutdown(void);
void update(float elapsed);
bool can_quit(void);

SGE_GAME_END

#endif // SGE_GAME_HPP

