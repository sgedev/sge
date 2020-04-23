/*
 *
 */
#ifndef SGE_SCENE_H
#define SGE_SCENE_H

#include <sge/common.h>

int sge_scene_init(void);
void sge_scene_shutdown(void);
void sge_scene_update(float elapsed);

#endif /* SGE_SCENE_H */

