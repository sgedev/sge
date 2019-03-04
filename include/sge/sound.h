//
//
#ifndef SGE_SOUND_H
#define SGE_SOUND_H

#include <sge/common.h>

#define SGE_SOUND_BEGIN SGE_BEGIN namespace sound {
#define SGE_SOUND_END } SGE_END

SGE_SOUND_BEGIN

bool init(void);
void shutdown(void);
void update(void);
void handle_event(const SDL_Event *event);

SGE_SOUND_END

#endif // SGE_SOUND_H
