/*
 *
 */
#ifndef SGE_GUI_H
#define SGE_GUI_H

#include <sge/common.h>

typedef struct sge_gui_base {
	struct sge_gui_base *parent;
	cx_list_node_t node;
	cx_list_t children;
} sge_gui_base_t;

typedef enum {
	SGE_GUI_TYPE_INVALID = 0,
	SGE_GUI_TYPE_WINDOW,
	SGE_GUI_TYPE_LABEL,
	SGE_GUI_TYPE_BUTTON,
	SGE_GUI_TYPE_EDIT,
	SGE_GUI_TYPE_PROGRESS,
	SGE_GUI_TYPE_
} sge_gui_type_t;

enum {
	SGE_GUI_WINDOW_FLAG_VISIBLE = 0x1,
};

#define SGE_GUI_WINDOW_NAME_MAX 64

typedef struct sge_gui_window {
	sge_gui_base_t base;
	char name[SGE_GUI_WINDOW_NAME_MAX];
	int flags;
	int x;
	int y;
	int width;
	int height;
} sge_gui_window_t;

typedef struct {
} sge_gui_callbacks_t;

int sge_gui_init(void);
void sge_gui_shutdown(void);
void sge_gui_handle_event(const SDL_Event *event);
void sge_gui_update(float elapsed, const sge_gui_callbacks_t *callbacks);
void sge_gui_draw(NVGcontext *nvg);

int sge_gui_window_init(sge_gui_window_t *window, sge_gui_window_t *parent, const char *name, int x, int y, int width, int height, int flags);
void sge_gui_window_fina(sge_gui_window_t *window);
void sge_gui_window_move(sge_gui_window_t *window, int x, int y);
void sge_gui_window_resize(sge_gui_window_t *window, int width, int height);
void sge_gui_window_set_parent(sge_gui_window_t *window, sge_gui_window_t *parent);
void sge_gui_window_show(sge_gui_window_t *window);
void sge_gui_window_hide(sge_gui_window_t *window);

#endif /* SGE_GUI_H */

