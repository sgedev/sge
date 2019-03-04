//
//
#ifndef SGE_GUI_H
#define SGE_GUI_H

#include <sge/gui/common.h>

SGE_GUI_BEGIN

bool init(void);
void shutdown(void);
void update(void);
void draw(void);
void handle_event(const SDL_Event *event);

SGE_GUI_END

#endif // SGE_GUI_H

