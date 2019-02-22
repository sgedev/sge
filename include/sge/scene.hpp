//
//
#ifndef SGL_SCENE_H
#define SGL_SCENE_H

#include "SGL_scene_common.h"

SGL_SCENE_BEGIN

bool Init(void);
void Shutdown(void);
void Update(float elapsed);
void Draw(float elapsed);
void HandleEvent(const SDL_Event *event);

SGL_SCENE_END

#endif // SGL_SCENE_H

