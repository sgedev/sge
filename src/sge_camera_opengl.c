/*
 *
 */
#include "glew/glew.h"

#include "sge_camera.h"

typedef struct {
	int foobar;
} sge_camera_opengl;

static sge_bool_t sge_glew_inited = SGE_FALSE;

static sge_bool_t sge_camera_opengl_init(sge_camera_t *camera)
{
	sge_camera_opengl *priv = sge_camera_to_priv(camera);
	SGE_ASSERT(priv != NULL);

	if (!sge_glew_inited) {
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			return SGE_FALSE;
		sge_glew_inited = SGE_TRUE;
	}

	return SGE_TRUE;
}

SGE_API void sge_camera_opengl_shutdown(sge_camera_t *camera)
{
	sge_camera_opengl *priv = sge_camera_to_priv(camera);
	SGE_ASSERT(priv != NULL);
}

SGE_API void sge_camera_opengl_draw(sge_camera_t *camera)
{
	sge_camera_opengl *priv = sge_camera_to_priv(camera);
	SGE_ASSERT(priv != NULL);
}

static sge_camera_type_t sge_camera_type_opengl = {
	.priv_size = sizeof(sge_camera_opengl),
	.init = sge_camera_opengl_init,
	.shutdown = sge_camera_opengl_shutdown,
	.draw = sge_camera_opengl_draw
};

SGE_API const sge_camera_type_t * const SGE_CAMERA_TYPE_OPENGL = &sge_camera_type_opengl;

