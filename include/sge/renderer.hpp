//
//
#ifndef SGE_RENDERER_HPP
#define SGE_RENDERER_HPP

#include <sge/renderer/common.hpp>

SGE_RENDERER_BEGIN

bool init(void);
void shutdown(void);
void draw(void);
void handle_event(const SDL_Event *event);

SGE_RENDERER_END

#endif // SGE_RENDERER_HPP
