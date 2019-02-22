//
//
#ifndef SGL_SOUND_H
#define SGL_SOUND_H

#include "SGL_common.h"

#define SGL_SOUND_BEGIN SGL_BEGIN namespace Sound {
#define SGL_SOUND_END } SGL_END

SGL_SOUND_BEGIN

bool Init(void);
void Shutdown(void);
void Update(float elapsed);
void HandleEvent(const SDL_Event *event);

SGL_SOUND_END

#endif // SGL_SOUND_H

