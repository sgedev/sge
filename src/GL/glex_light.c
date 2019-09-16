/*
 *
 */
#include "glex_common.h"

GLboolean glexInitLight(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);

	return GL_TRUE;
}

void glexShutdownLight(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);
}

GLEX_API GLEXLight *glexCreateLight(GLEXLightType type)
{
	GLEX_ASSERT(glex != NULL);

	return NULL;
}

GLEX_API void glexDestroyLight(GLEXLight *light)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(light != NULL);
}

GLEX_API void glexEnableLight(GLEXLight *light)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(light != NULL);
}

GLEX_API void glexDisableLight(GLEXLight *light)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(light != NULL);
}

