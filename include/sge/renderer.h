//
//
#ifndef SGE_RENDERER_H
#define SGE_RENDERER_H

#include <sge/renderer/common.h>

SGE_RENDERER_BEGIN

bool init(void);
void shutdown(void);
void update(void);
void draw(void);
void handle_event(const SDL_Event *event);

SGE_RENDERER_END

#endif // SGE_RENDERER_H
