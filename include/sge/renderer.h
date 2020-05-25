/*
 *
 */
#ifndef SGE_RENDERER_H
#define SGE_RENDERER_H

#include <lua.h>

#include <sge/common.h>

SGE_BEGIN_DECLS

void sge_renderer_init(lua_State *L);
void sge_renderer_shutdown(void);
bool sge_renderer_start(void);
void sge_renderer_stop(void);

SGE_END_DECLS

#endif /* SGE_RENDERER_H */
