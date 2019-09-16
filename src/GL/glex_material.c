/*
 *
 */
#include "glex_common.h"

GLboolean glexInitMaterial(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);

	return GL_TRUE;
}

void glexShutdownMaterial(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);
}

GLEX_API GLEXMaterial *glexCreateMaterial(const GLEXMaterialInfo *info)
{
	GLEXMaterial *material;

	GLEX_ASSERT(glex != NULL);

	material = glexAllocMemory(sizeof(GLEXMaterial));
	if (material == NULL)
		return NULL;

	return material;
}

GLEX_API void glexDestroyMaterial(GLEXMaterial *material)
{
	GLEX_ASSERT(glex != NULL);

	if (material == NULL)
		return;

	//glexListDel(&glex->material
}
