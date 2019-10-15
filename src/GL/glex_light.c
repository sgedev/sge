/*
 *
 */
#include "glex_light.h"

typedef struct {
	int foobar;
} GLEXLight;

static int glexLightInit(void *obj)
{
	GLEXLight *light = obj;

	return 0;
}

static void glexLightFina(void *obj)
{
	GLEXLight *libhg = obj;
}

static const GLEXType glexLightTypeInstance = {
	.name = "GLEXLight",
	.size = sizeof(GLEXLight),
	.init = glexLightInit,
	.fina = glexLightFina
};

const GLEXType * const glexLightType = &glexLightTypeInstance;

