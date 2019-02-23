//
//
#ifndef SGE_SOUND_H
#define SGE_SOUND_H

#include <sge/common.h>

#define SGE_SOUND_BEGIN SGE_BEGIN namespace Sound {
#define SGE_SOUND_END } SGE_END

SGE_SOUND_BEGIN

bool Init(void);
void Shutdown(void);
void Update(void);
void HandleEvent(const SDL_Event *event);

SGE_SOUND_END

#endif // SGE_SOUND_H
