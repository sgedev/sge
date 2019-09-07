/*
 *
 */
#include "glex_material.h"

struct GLEXMaterial_ {
	cx_list_node_t node;
};

GLboolean glexInitMaterial(void)
{
	return GL_TRUE;
}

void glexShutdownMaterial(void)
{
}

GLEX_API GLEXMaterial *glexCreateMaterial(const GLEXMaterialInfo *info)
{
	GLEXMaterial *material;

	material = glexAllocMemory(sizeof(GLEXMaterial));
	if (material == NULL)
		return NULL;

	return material;
}

GLEX_API void glexDestroyMaterial(GLEXMaterial *material)
{
	if (material == NULL)
		return;

	//glexListDel(&glex->material
}
