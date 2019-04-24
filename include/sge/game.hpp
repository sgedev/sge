//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <sge/common.hpp>

#define SGE_GAME_BEGIN SGE_BEGIN namespace game {
#define SGE_GAME_END } SGE_END

SGE_GAME_BEGIN

bool init(void);
void shutdown(void);
void update(float elapsed);

SGE_GAME_END

#endif // SGE_GAME_HPP

