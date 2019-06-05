//
//
#ifndef SGE_EDITOR_HPP
#define SGE_EDITOR_HPP

#include <sge/editor/common.hpp>

SGE_EDITOR_BEGIN

bool init(void);
void shutdown(void);
bool handle_event(const SDL_Event &event);
void update(float elapsed);
void draw(void);

SGE_EDITOR_END

#endif // SGE_EDITOR_HPP

