//
//
#ifndef SGL_RENDERER_H
#define SGL_RENDERER_H

#include "SGL_renderer_common.h"
#include "SGL_renderer_object.h"

SGL_RENDERER_BEGIN

bool Init(void);
void Shutdown(void);
void Draw(float elapsed);
void HandleEvent(const SDL_Event *event);
void AddObject(Object *obj);
void RemoveObject(Object *obj);

SGL_RENDERER_END

#endif // SGL_RENDERER_H
