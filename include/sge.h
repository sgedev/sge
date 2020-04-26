/*
 *
 */
#ifndef SGE_H
#define SGE_H

#include <sge/common.h>
#include <sge/window.h>
#include <sge/input.h>
#include <sge/scene.h>
#include <sge/vm.h>

SGE_BEGIN_DECLS

int sge_init(const char *root);
void sge_shutdown(void);
void sge_handle_event(const SDL_Event *event);
void sge_frame(float elapsed);

SGE_END_DECLS

#endif /* SGE_H */

