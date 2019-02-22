//
//
#ifndef SGL_GUI_H
#define SGL_GUI_H

#include "SGL_gui_common.h"

SGL_GUI_BEGIN

bool Init(void);
void Shutdown(void);
void Draw(float elapsed);
void HandleEvent(const SDL_Event *event);

SGL_GUI_END

#endif // SGL_GUI_H

