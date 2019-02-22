//
//
#ifndef SGE_PHYSICS_HPP
#define SGE_PHYSICS_HPP

#include <sge/physics/common.hpp>

SGE_PHYSICS_BEGIN

bool init(void);
void shutdown(void);
void update(float elapsed);
void handle_event(const SDL_Event *event);

SGE_PHYSICS_END

#endif // SGE_PHYSICS_HPP

