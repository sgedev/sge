//
//
#ifndef SGE_RENDERER_HPP
#define SGE_RENDERER_HPP

#include <sge/common.hpp>

#define SGE_RENDERER_BEGIN SGE_BEGIN namespace Renderer {
#define SGE_RENDERER_END } SGE_END

SGE_RENDERER_BEGIN

bool init(void);
void shutdown(void);
bool handleEvent(const SDL_Event &event);
bool beginFrame(void);
void endFrame(void);

SGE_RENDERER_END

#endif // SGE_RENDERER_HPP
