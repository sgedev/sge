/*
 *
 */
#ifndef SGE_OBJECT_H
#define SGE_OBJECT_H

#include <cx/list.h>

#include <sge/common.h>

SGE_BEGIN_DECLS

typedef struct sge_object sge_object_t;

bool sge_object_init(void);
void sge_object_shutdown(void);
void sge_object_update_all(float elapsed);
void sge_object_draw_all(void);
sge_object_t *sge_object_new(const char *name, sge_object_t *parent);
sge_object_t *sge_object_get_root(void);
sge_object_t *sge_object_get_parent(sge_object_t *obj);

SGE_END_DECLS

#endif /* SGE_OBJECT_H */

