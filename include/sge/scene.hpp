//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <sge/scene/common.hpp>

SGE_SCENE_BEGIN

bool init(void);
void shutdown(void);
void update(float elapsed);
void draw(void);
void handle_event(const SDL_Event *event);

SGE_SCENE_END

#endif // SGE_SCENE_HPP

