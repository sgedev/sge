/*
 *
 */
#ifndef SGE_PHYSICS_H
#define SGE_PHYSICS_H

#include <uv.h>
#include <lua.h>

#include <sge/common.h>

SGE_BEGIN_DECLS

void sge_physics_export(lua_State *L);
bool sge_physics_init(void);
void sge_physics_shutdown(void);
void sge_physics_update(float elapsed);

SGE_END_DECLS

#endif /* SGE_PHYSICS_H */
