//
//
#include "glex_mesh.h"

typedef struct {
	int foobar;
} GLEXMesh;

static int glexMeshInit(void *obj)
{
	GLEXMesh *mesh = obj;

	return 0;
}

static void glexMeshFina(void *obj)
{
	GLEXMesh *mesh = obj;
}

static const GLEXType glexMeshTypeInstance = {
	.name = "GLEXMesh",
	.size = sizeof(GLEXMesh),
	.init = glexMeshInit,
	.fina = glexMeshFina
};

const GLEXType * const glexMeshType = &glexMeshTypeInstance;

