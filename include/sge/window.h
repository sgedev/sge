/*
 *
 */
#ifndef SGE_WINDOW_H
#define SGE_WINDOW_H

#include <lua.h>

#include <sge/common.h>

SGE_BEGIN_C_DECLS

void sge_window_export(lua_State *L);
bool sge_window_init(void);
void sge_window_shutdown(void);
void sge_window_handle_event(const SDL_WindowEvent *evt);
bool sge_window_begin_draw(void);
void sge_window_end_draw(void);

SGE_END_C_DECLS

#endif /* SGE_WINDOW_H */
