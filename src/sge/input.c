/*
 *
 */
#include <sge/input.h>

typedef struct {
	int pressed;
} sge_input_key_state_t;

int sge_input_init(void)
{
}

void sge_input_shutdown(void)
{
}

void sge_input_handle_mouse_move_event(const SDL_MouseMotionEvent *event)
{
}

void sge_input_handle_mouse_button_event(const SDL_MouseButtonEvent *event)
{
}

void sge_input_handle_key_event(const SDL_KeyboardEvent *event)
{
}

void sge_input_update(float elapsed)
{
}

