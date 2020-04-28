/*
 *
 */
#ifndef SGE_SCENE_H
#define SGE_SCENE_H

#include <GL/glex.h>

#include <sge/common.h>

SGE_BEGIN_DECLS

typedef struct sge_object sge_object_t;

sge_object_t *sge_object_new(const char *name);
void sge_object_destroy(sge_object_t *obj);
bool sge_object_is_enabled(sge_object_t *obj);
void sge_object_enable(sge_object_t *obj);
void sge_object_disable(sge_object_t *obj);
bool sge_object_is_visibled(sge_object_t *obj);
void sge_object_show(sge_object_t *obj);
void sge_object_hide(sge_object_t *obj);
const char *sge_object_get_name(sge_object_t *obj);
void sge_object_set_name(sge_object_t *obj, const char *name);
const hmm_vec3 *sge_object_get_pos(sge_object_t *obj);
void sge_object_set_pos(sge_object_t *obj, float x, float y, float z);
const hmm_vec3 *sge_object_get_scale(sge_object_t *obj);
void sge_object_set_scale(sge_object_t *obj, float x, float y, float z);
const hmm_quaternion *sge_object_get_rotation(sge_object_t *obj);
void sge_object_set_rotation(sge_object_t *obj, float angle, float x, float y, float z);
void sge_object_set_surface(sge_object_t *obj, const char *res_path);

const hmm_vec3 *sge_camera_vpos(void);
void sge_camera_set_pos(float x, float y, float z);
const hmm_vec3 *sge_camera_get_scale(void);
void sge_camera_set_scale(float x, float y, float z);
const hmm_quaternion *sge_camera_get_rotation(void);
void sge_camera_set_rotation(float angle, float x, float y, float z);
const hmm_mat4 *sge_camera_get_projection(void);
void sge_camera_set_projection(const hmm_mat4 *p);

bool sge_scene_init(void);
void sge_scene_shutdown(void);
void sge_scene_update(float elapsed);
void sge_scene_draw_glex(void);

SGE_END_DECLS

#endif /* SGE_SCENE_H */

