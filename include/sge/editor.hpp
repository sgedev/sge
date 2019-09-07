//
//
#ifndef SGE_EDITOR_HPP
#define SGE_EDITOR_HPP

#include <sge/common.hpp>

#define SGE_EDITOR_BEGIN SGE_BEGIN namespace editor {
#define SGE_EDITOR_END } SGE_END

SGE_EDITOR_BEGIN

bool init(void);
void shutdown(void);
void handle_event(const SDL_Event *event);
void update(float elapsed);
void draw(void);

SGE_EDITOR_END

#endif // SGE_EDITOR_HPP

