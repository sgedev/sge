/*
 *
 */
#ifndef SGE_CAMERA_H
#define SGE_CAMERA_H

#include "sge_common.h"

struct sge_camera_struct {
	const sge_camera_type_t *type;
};

static inline void *sge_camera_to_priv(sge_camera_t *camera)
{
	SGE_ASSERT(camera != NULL);
	SGE_ASSERT(camera->type != NULL);

	if (camera->type->priv_size < 1)
		return NULL;

	return SGE_PMOVB(camera, sizeof(sge_camera_t));
}

#endif /* SGE_CAMERA_H */

