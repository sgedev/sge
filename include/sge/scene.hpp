//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <sge/scene/common.hpp>
#include <sge/scene/renderer.hpp>
#include <sge/scene/camera.hpp>

SGE_SCENE_BEGIN

bool init(void);
void shutdown(void);
bool load(const char *path);
void update(void);
void draw(void);

SGE_SCENE_END

#endif // SGE_SCENE_HPP

