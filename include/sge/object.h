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

typedef struct sge_object sge_object_t;
typedef struct sge_object_new_watch sge_object_new_watch_t;
typedef void (*sge_object_new_watch_func_t)(sge_object_t *obj, void *data);
typedef struct sge_object_watch sge_object_watch_t;

typedef struct {
    void (*destroy)(sge_object_t *obj, void *data);
    void (*set_enabled)(sge_object_t *obj, bool v, void *data);
    void (*set_visibled)(sge_object_t *obj, bool v, void *data);
    void (*set_name)(sge_object_t *obj, const char *name, void *data);
    void (*set_pos)(sge_object_t *obj, const cx_vec3_t *p, void *data);
    void (*set_scale)(sge_object_t *obj, const cx_vec3_t *p, void *data);
    void (*set_rotation)(sge_object_t *obj, const cx_quat_t *p, void *data);
    void (*set_mass)(sge_object_t *obj, float v, void *data);
    void (*set_property)(sge_object_t *obj, const char *prop, const char *value, void *data);
} sge_object_watch_funcs_t;

void sge_object_export(lua_State *L);
bool sge_object_init(void);
void sge_object_shutdown(void);
sge_object_new_watch_t *sge_object_add_new_watch(sge_object_new_watch_func_t func, void *data);
void sge_object_remove_new_watch(sge_object_new_watch_t *new_watch);
sge_object_watch_t *sge_object_add_watch(sge_object_t *obj, const sge_object_watch_funcs_t *funcs, void *data);
void sge_object_remove_watch(sge_object_t *obj, sge_object_watch_t *watch);
void sge_object_set_enabled(sge_object_t *obj, bool v);
void sge_object_set_visibled(sge_object_t *obj, bool v);
void sge_object_set_name(sge_object_t *obj, const char *name);
void sge_object_set_pos(sge_object_t *obj, const cx_vec3_t *p);
void sge_object_set_scale(sge_object_t *obj, const cx_vec3_t *p);
void sge_object_set_rotation(sge_object_t *obj, const cx_quat_t *p);
void sge_object_set_mass(sge_object_t *obj, float v);
void sge_object_set_property(sge_object_t *obj, const char *prop, const char *value);

SGE_END_C_DECLS

#endif /* SGE_OBJECT_H */
