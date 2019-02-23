//
//
#ifndef SGE_PHYSICS_H
#define SGE_PHYSICS_H

#include <sge/physics/common.h>

SGE_PHYSICS_BEGIN

bool Init(void);
void Shutdown(void);
void Update(void);
void HandleEvent(const SDL_Event *event);

SGE_PHYSICS_END

#endif // SGE_PHYSICS_H
