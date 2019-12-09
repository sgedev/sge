//
//
#ifndef SGE_HPP
#define SGE_HPP

#include <sge/common.hpp>
#include <sge/renderer.hpp>

SGE_BEGIN

bool init(void);
void shutdown(void);
const char *lastError(void);
bool handleEvent(const SDL_Event &evt);
void update(float elapsed);

SGE_END

#endif // SGE_HPP
