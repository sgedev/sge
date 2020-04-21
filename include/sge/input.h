/*
 *
 */
#ifndef SGE_INPUT_H
#define SGE_INPUT_H

#include <sge/common.h>

int SGE_InitInput(void);
void SGE_ShutdownInput(void);
int SGE_HandleMouseMoveEvent(const SDL_MouseMotionEvent *event);
int SGE_HandleMouseButtonEvent(const SDL_MouseButtonEvent *event);
int SGE_HandleKeyEvent(const SDL_KeyboardEvent *event);
void SGE_UpdateInput(float elapsed);

#endif /* SGE_INPUT_H */

