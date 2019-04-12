//
//
#ifndef SGE_SCENE_RENDERER_HPP
#define SGE_SCENE_RENDERER_HPP

#include <sge/scene/common.hpp>

#define SGE_SCENE_RENDERER_BEGIN SGE_SCENE_BEGIN namespace renderer {
#define SGE_SCENE_RENDERER_END } SGE_SCENE_END

SGE_SCENE_RENDERER_BEGIN

bool init(void);
void shutdown(void);
void begin(void);
void end(void);

SGE_SCENE_RENDERER_END

#endif // SGE_SCENE_RENDERER_HPP

