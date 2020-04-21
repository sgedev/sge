/*
 *
 */
#include <sge/input.h>

typedef struct {
	int pressed;
	
} SGE_KeyState;

int SGE_InitInput(void)
{
}

void SGE_ShutdownInput(void)
{
}

int SGE_HandleMouseMoveEvent(const SDL_MouseMotionEvent *event)
{
	return -1;
}

int SGE_HandleMouseButtonEvent(const SDL_MouseButtonEvent *event)
{
	return -1;
}

int SGE_HandleKeyEvent(const SDL_KeyboardEvent *event)
{
	return -1;
}

void SGE_UpdateInput(float elapsed)
{

}

