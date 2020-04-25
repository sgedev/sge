/*
 *
 */
#ifndef SGE_SCENE_H
#define SGE_SCENE_H

#include <sge/common.h>

int sge_scene_init(void);
void sge_scene_shutdown(void);
void sge_scene_update(float elapsed);
void sge_scene_draw(void);
void sge_scene_set_view_matrix(const hmm_mat4 *m);
void sge_scene_set_projection_matrix(const hmm_mat4 *m);

#endif /* SGE_SCENE_H */

