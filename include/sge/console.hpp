//
//
#ifndef SGE_CONSOLE_HPP
#define SGE_CONSOLE_HPP

#include <sge/common.hpp>

#define SGE_CONSOLE_BEGIN SGE_BEGIN namespace console {
#define SGE_CONSOLE_END } SGE_END

SGE_CONSOLE_BEGIN

bool init(void);
void shutdown(void);
bool handle_event(const SDL_Event &event);
void update(float elapsed);
void draw(void);

SGE_CONSOLE_END

#endif // SGE_CONSOLE_HPP

