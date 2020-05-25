/*
 *
 */
#ifndef SGE_OBJECT_H
#define SGE_OBJECT_H

#include <lua.h>

#include <sge/common.h>
#include <sge/list.h>

SGE_BEGIN_DECLS

#define SGE_OBJECT_NAME_MAX 64

typedef struct sge_object sge_object_t;

typedef struct {
    void (*object_added)(sge_object_t *object);
    void (*object_removed)(sge_object_t *object); 

    /* private */
    sge_list_node_t node;
} sge_object_watch_t;

void sge_object_export(lua_State *L);
bool sge_object_init(void);
void sge_object_shutdown(void);
const char *sge_object_get_name(sge_object_t *object);
void sge_object_set_name(sge_object_t *object, const char *name);
sge_object_t *sge_object_get_parent(sge_object_t *object);
void sge_object_set_parent(sge_object_t *object, sge_object_t *parent);


void sge_object_watch_add(sge_object_watch_t *watch);
void sge_object_watch_remove(sge_object_watch_t *watch);

SGE_END_DECLS

#endif /* SGE_OBJECT_H */
