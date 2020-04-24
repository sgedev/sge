/*
 *
 */
#ifndef NGUI_H
#define NGUI_H

#include <ngui/common.h>
#include <ngui/window.h>
#include <ngui/button.h>

int nvgui_init(void);
void nvgui_shutdown(void);
void nvgui_update(float elapsed);
void nvgui_render(NVGcontext *nvg);
void nvgui_handle_key_event();
void nvgui_handle_mouse_move_event();
void nvgui_handle_mouse_button_event();

#endif /* NGUI_H */

