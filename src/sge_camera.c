/*
 *
 */
#include "sge_camera.h"

SGE_API sge_camera_t *sge_camera_new(const sge_camera_type_t *type)
{
	sge_camera_t *camera;

	if (type == NULL)
		goto bad0;

	camera = malloc(sizeof(sge_camera_t) + type->priv_size);
	if (camera == NULL)
		goto bad0;

	if (type->init != NULL) {
		if (!type->init(camera))
			goto bad1;
	}

	camera->type = type;

	return camera;

bad1:
	free(camera);

bad0:
	return NULL;
}

SGE_API void sge_camera_destroy(sge_camera_t *camera)
{
	if (camera == NULL)
		return;

	SGE_ASSERT(camera->type != NULL);

	if (camera->type->shutdown != NULL)
		camera->type->shutdown(camera);

	free(camera);
}

SGE_API void sge_camera_draw(sge_camera_t *camera)
{
	if (camera == NULL)
		return;

	SGE_ASSERT(camera->type != NULL);

	if (camera->type->draw != NULL)
		camera->type->draw(camera);
}

