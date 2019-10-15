//
//
#include "glex_material.h"

typedef struct {
	int foobar;
} GLEXMaterial;

static int glexMaterialInit(void *obj)
{
	GLEXMaterial *material = obj;

	return 0;
}

static void glexMaterialFina(void *obj)
{
	GLEXMaterial *material = obj;
}

static const GLEXType glexMaterialTypeInstance = {
	.name = "GLEXMaterial",
	.size = sizeof(GLEXMaterial),
	.init = glexMaterialInit,
	.fina = glexMaterialFina
};

const GLEXType * const glexMaterialType = &glexMaterialTypeInstance;

