/*
 *
 */
#ifndef SGE_VM_H
#define SGE_VM_H

#include <nanovg.h>

#include <sge/common.h>

SGE_BEGIN_DECLS

typedef struct {
	void (*exit)(void);
	int (*get_fps)(void);
	int (*get_fps_max)(void);
	void (*set_fps_max)(int v);
	void (*toggle_show_fps)(void);
	void (*set_window_title)(const char *name);
	void (*move_window)(int x, int y);
	void (*resize_window)(int width, int height);
	void (*get_window_rect)(int *x, int *y, int *width, int *height);
	void (*show_window)(void);
	void (*hide_window)(void);
} sge_vm_traps_t;

bool sge_vm_init(const char *root);
void sge_vm_shutdown(void);
void sge_vm_handle_event(const SDL_Event *event);
void sge_vm_update(float elapsed, const sge_vm_traps_t *traps);
void sge_vm_draw_glex(void);
void sge_vm_draw_nvg(NVGcontext *nvg);

SGE_END_DECLS

#endif /* SGE_VM_H */

