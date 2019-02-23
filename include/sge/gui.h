//
//
#ifndef SGE_GUI_H
#define SGE_GUI_H

#include <sge/gui/common.h>

SGE_GUI_BEGIN

bool Init(void);
void Shutdown(void);
void Update(void);
void Draw(void);
void HandleEvent(const SDL_Event *event);

SGE_GUI_END

#endif // SGE_GUI_H

