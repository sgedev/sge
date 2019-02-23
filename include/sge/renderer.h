//
//
#ifndef SGE_RENDERER_H
#define SGE_RENDERER_H

#include <sge/renderer/common.h>

SGE_RENDERER_BEGIN

bool Init(void);
void Shutdown(void);
void Update(void);
void Draw(void);
void HandleEvent(const SDL_Event *event);

SGE_RENDERER_END

#endif // SGE_RENDERER_H
