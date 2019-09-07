//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <sge/common.hpp>

#define SGE_SCENE_BEGIN SGE_BEGIN namespace scene {
#define SGE_SCENE_END } SGE_END

SGE_SCENE_BEGIN

bool init(void);
void shutdown(void);
void update(float elapsed);
void draw(void);

SGE_SCENE_END

#endif // SGE_SCENE_HPP

