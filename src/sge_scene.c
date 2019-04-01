/*
 *
 */
#include "sge_camera.h"
#include "sge_scene.h"

struct sge_scene_struct {
	int foobar;
};

SGE_API sge_scene_t *sge_scene_new(sge_fs_t *fs, const char *dirname)
{
	sge_scene_t *scene;

	if (fs == NULL || dirname == NULL)
		return NULL;

	scene = malloc(sizeof(sge_scene_t));
	if (scene == NULL)
		return NULL;

	return scene;
}

SGE_API void sge_scene_destroy(sge_scene_t *scene)
{
	if (scene == NULL)
		return;

	free(scene);
}

SGE_API void sge_scene_update(sge_scene_t *scene, float elapsed)
{
	if (scene == NULL)
		return;
}

SGE_API void sge_scene_draw(sge_scene_t *scene, sge_camera_t *camera)
{
	if (scene == NULL || camera == NULL)
		return;

#if 0
	sge_camera_add(camera, surfaces from scene);
	...
#endif
}

