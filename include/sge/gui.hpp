//
//
#ifndef SGE_GUI_HPP
#define SGE_GUI_HPP

#include <sge/gui/common.hpp>

SGE_GUI_BEGIN

bool init(void);
void shutdown(void);
void handle_event(const SDL_Event &event);
void update(void);
void draw(void);

SGE_GUI_END

#endif // SGE_GUI_HPP

