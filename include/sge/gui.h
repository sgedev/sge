/*
 *
 */
#ifndef SGE_GUI_H
#define SGE_GUI_H

#include <lua.h>

#include <sge/gui/common.h>

SGE_BEGIN_C_DECLS

void sge_gui_export(lua_State *L);
bool sge_gui_init(void);
void sge_gui_shutdown(void);
void sge_gui_handle_event(const SDL_Event *evt);
void sge_gui_draw(float elapsed);

SGE_END_C_DECLS

#endif /* SGE_RENDERER_H */
