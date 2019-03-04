//
//
#ifndef SGE_PHYSICS_H
#define SGE_PHYSICS_H

#include <sge/physics/common.h>

SGE_PHYSICS_BEGIN

bool init(void);
void shutdown(void);
void update(void);
void handle_event(const SDL_Event *event);

SGE_PHYSICS_END

#endif // SGE_PHYSICS_H
