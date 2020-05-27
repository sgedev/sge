/*
 *
 */
#ifndef SGE_RENDERER_H
#define SGE_RENDERER_H

#include <lua.h>

#include <sge/renderer/common.h>

SGE_BEGIN_C_DECLS

void sge_renderer_export(lua_State *L);
bool sge_renderer_init(void);
void sge_renderer_shutdown(void);
void sge_renderer_draw(float elapsed);

SGE_END_C_DECLS

#endif /* SGE_RENDERER_H */
