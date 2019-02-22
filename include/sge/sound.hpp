//
//
#ifndef SGE_SOUND_HPP
#define SGE_SOUND_HPP

#include <sge/common.hpp>

#define SGE_SOUND_BEGIN SGE_BEGIN namespace sound {
#define SGE_SOUND_END } SGE_END

SGE_SOUND_BEGIN

bool init(void);
void shutdown(void);
void update(float elapsed);
void handle_event(const SDL_Event *event);

SGE_SOUND_END

#endif // SGE_SOUND_HPP
