//
//
#ifndef SGL_PHYSICS_H
#define SGL_PHYSICS_H

#include "SGL_physics_common.h"
#include "SGL_physics_object.h"

SGL_PHYSICS_BEGIN

bool Init(void);
void Shutdown(void);
void Update(float elapsed);
void HandleEvent(const SDL_Event *event);
void AddObject(Object *obj);
void RemoveObject(Object *obj);

SGL_PHYSICS_END

#endif // SGL_PHYSICS_H

