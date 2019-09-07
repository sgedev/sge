/*
 *
 */
#include "glex_light.h"

struct GLEXLight_ {
	cx_list_node_t node;
	GLEXLightType type;
};

GLboolean glexInitLight(void)
{
	return GL_TRUE;
}

void glexShutdownLight(void)
{
}

GLEX_API GLEXLight *glexCreateLight(GLEXLightType type)
{
	return NULL;
}

GLEX_API void glexDestroyLight(GLEXLight *light)
{
	GLEX_ASSERT(light != NULL);
}

GLEX_API void glexEnableLight(GLEXLight *light)
{
	GLEX_ASSERT(light != NULL);
}

GLEX_API void glexDisableLight(GLEXLight *light)
{
	GLEX_ASSERT(light != NULL);
}
