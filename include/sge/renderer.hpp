//
//
#ifndef SGE_RENDERER_HPP
#define SGE_RENDERER_HPP

#include <imgui.h>

#include <sge/renderer/common.hpp>

SGE_RENDERER_BEGIN

bool init(void);
void shutdown(void);
bool handleEvent(const SDL_Event &evt);
bool beginFrame(void);
void endFrame(void);

SGE_RENDERER_END

#endif // SGE_RENDERER_HPP

