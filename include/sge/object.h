/*
 *
 */
#ifndef SGE_OBJECT_H
#define SGE_OBJECT_H

#include <lua.h>

#include <sge/common.h>

SGE_BEGIN_DECLS

typedef struct sge_object sge_object_t;

void sge_object_init(lua_State *L);
void sge_object_shutdown(void);
sge_object_t *sge_object_new(const char *name, sge_object_t *parent);
sge_object_t *sge_object_ref(sge_object_t *object);
sge_object_t *sge_object_dup(sge_object_t *object);
void sge_object_destroy(sge_object_t *object);
const char *sge_object_get_name(sge_object_t *object);
void sge_object_set_name(sge_object_t *object, const char *name);

SGE_END_DECLS

#endif /* SGE_OBJECT_H */
