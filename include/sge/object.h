/*
 *
 */
#ifndef SGE_OBJECT_H
#define SGE_OBJECT_H

#include <lua.h>

#include <cx/list.h>
#include <cx/sigslot.h>

#include <sge/common.h>

SGE_BEGIN_C_DECLS

#define SGE_OBJECT_NAME_MAX 64

#define SGE_OBJECT_FLAG_ENABLED 0x1
#define SGE_OBJECT_FLAG_VISIBLED 0x2
#define SGE_OBJECT_FLAG_MOVABLED 0x4

typedef struct sge_object sge_object_t;

cx_sigslot_define(sge_object, sge_object_t *object);
cx_sigslot_define(sge_object_property, const char *key, const char *value);

struct sge_object {
    cx_sig(int) set_flags;
    cx_sig(str) set_name;
    cx_sig(vec3) set_pos;
    cx_sig(vec3) set_scale;
    cx_sig(quat) set_rotate;
    cx_sig(sge_object_property) set_property;

	cx_list_node_t node;
	cx_list_t children;
	sge_object_t *parent;
};


extern cx_sig(sge_object) sge_object_added;
extern cx_sig(sge_object) sge_object_removed;

static SGE_INLINE sge_object_t *sge_object_get_parent(sge_object_t *object)
{
    SGE_ASSERT(object != NULL);
    return object->parent;
}

void sge_object_export(lua_State *L);
bool sge_object_init(void);
void sge_object_shutdown(void);
sge_object_t *sge_object_get_root(void);
void sge_object_set_parent(sge_object_t *object, sge_object_t *parent);
sge_object_t *sge_object_get_first_child(sge_object_t *object);
sge_object_t *sge_object_get_next_sibling(sge_object_t *object);

SGE_END_C_DECLS

#endif /* SGE_OBJECT_H */
