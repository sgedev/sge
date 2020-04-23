/*
 *
 */
#ifndef SGE_WINDOW_H
#define SGE_WINDOW_H

#include <sge/common.h>

typedef struct {
	void (*draw_3d)(void);
	void (*draw_2d)(NVGcontext *nvg);
} sge_window_drawer_t;

int sge_window_init(void);
void sge_window_shutdown(void);
void sge_window_handle_event(const SDL_WindowEvent *event);
void sge_window_update(float elapsed, const sge_window_drawer_t *drawer);

#endif /* SGE_WINDOW_H */

