//
//
#ifndef SGE_GUI_HPP
#define SGE_GUI_HPP

#include <sge/gui/common.hpp>
#include <sge/gui/widget.hpp>
#include <sge/gui/window.hpp>

SGE_GUI_BEGIN

bool init(void);
void shutdown(void);
bool handle_event(const SDL_Event &event);
void update(float elapsed);
void draw(void);

SGE_GUI_END

#endif // SGE_GUI_HPP

