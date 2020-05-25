/*
 *
 */
#ifndef SGE_PHYSICS_H
#define SGE_PHYSICS_H

#include <lua.h>

#include <sge/common.h>

SGE_BEGIN_DECLS

void sge_physics_init(lua_State *L);
void sge_physics_shutdown(void);
bool sge_physics_start(void);
void sge_physics_stop(void);

SGE_END_DECLS

#endif /* SGE_PHYSICS_H */
