/*
 *
 */
#ifndef SGE_PHYSICS_H
#define SGE_PHYSICS_H

#include <sge/common.h>
#include <sge/object.h>

SGE_BEGIN_DECLS

int sge_physics_init(void);
void sge_physics_shutdown(void);
void sge_physics_update(float elapsed);

SGE_END_DECLS

#endif /* SGE_PHYSICS_H */

