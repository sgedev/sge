/*
 *
 */
#ifndef SGE_WINDOW_H
#define SGE_WINDOW_H

#include <sge/common.h>

typedef struct {
	void (*Draw2D)(NVGcontext *nvg);
	void (*Draw3D)(void);
} SGE_WindowDrawer;

int SGE_InitWindow(void);
void SGE_ShutdownWindow(void);
int SGE_HandleWindowEvent(const SDL_WindowEvent *event);
void SGE_RenderWindow(const SGE_WindowDrawer *drawer);

#endif /* SGE_WINDOW_H */

