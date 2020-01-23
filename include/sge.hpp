//
//
#ifndef SGE_HPP
#define SGE_HPP

#include <sge/common.hpp>

SGE_BEGIN

bool init(void);
void shutdown(void);
bool handleEvent(const SDL_Event &event);
void frame(float elapsed);

SGE_END

#endif // SGE_HPP
