/*
 *
 */
#include "glex_common.h"

static unsigned int glexAllocStaticVertexBuffer(unsigned int size)
{
	return 0;
}

static unsigned int glexAllocStaticIndexBuffer(unsigned int size)
{
	return 0;
}

static unsigned int glexAllocDynamicVertexBuffer(unsigned int size)
{
	return 0;
}

static unsigned int glexAllocDynamicIndexBuffer(unsigned int size)
{
	return 0;
}

GLboolean glexInitMesh(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);

	glGenBuffers(1, &context->static_vertex_buffer);
	glGenBuffers(1, &context->static_index_buffer);
	glGenBuffers(1, &context->dynamic_vertex_buffer);
	glGenBuffers(1, &context->dynamic_index_buffer);

	return GL_TRUE;
}

void glexShutdownMesh(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);

	if (context->static_vertex_buffer) {
		glDeleteBuffers(1, &context->static_vertex_buffer);
		context->static_vertex_buffer = 0;
	}

	if (context->static_index_buffer) {
		glDeleteBuffers(1, &context->static_index_buffer);
		context->static_index_buffer = 0;
	}

	if (context->dynamic_vertex_buffer) {
		glDeleteBuffers(1, &context->dynamic_vertex_buffer);
		context->dynamic_vertex_buffer = 0;
	}

	if (context->dynamic_index_buffer) {
		glDeleteBuffers(1, &context->dynamic_index_buffer);
		context->dynamic_index_buffer = 0;
	}
}

GLEX_API GLEXMesh *glexCreateMesh(const GLEXMeshInfo *info)
{
	GLEXMesh* mesh;
	unsigned int vertex_size;
	unsigned int vertex_buffer_size;
	unsigned int index_buffer_size;

	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(info != NULL);
	GLEX_ASSERT(info->count > 0);

	mesh = glexAllocMemory(sizeof(GLEXMesh));
	if (mesh == NULL)
		goto bad0;

	vertex_size = sizeof(float) * 3;

	if (info->flags & GLEX_MESH_FLAG_RGB)
		vertex_size += sizeof(float) * 3;

	if (info->flags & GLEX_MESH_FLAG_RGB)
		vertex_size += sizeof(float) * 3;

	if (info->flags & GLEX_MESH_FLAG_UV)
		vertex_size += sizeof(float) * 2;

	vertex_buffer_size = vertex_size * info->count;

	if (info->flags & GLEX_MESH_FLAG_DYNAMIC)
		mesh->first_vertex = glexAllocDynamicVertexBuffer(vertex_buffer_size);
	else
		mesh->first_vertex = glexAllocStaticVertexBuffer(vertex_buffer_size);
	if (mesh->first_vertex = ~0U)
		goto bad1;

	if (info->flags & GLEX_MESH_FLAG_INDEXED) {
		index_buffer_size = sizeof(unsigned int) * info->count;
		if (info->flags & GLEX_MESH_FLAG_DYNAMIC)
			mesh->first_element = glexAllocDynamicIndexBuffer(index_buffer_size);
		else
			mesh->first_element = glexAllocStaticIndexBuffer(index_buffer_size);
		if (mesh->first_element == ~0U)
			goto bad2;
	}

	cx_list_node_reset(&mesh->node);

	mesh->opening = GL_FALSE;
	mesh->count = info->count;
	mesh->flags = info->flags;

	return mesh;

bad2:
	//glexFreeVertexBuffer(mesh->first_vertex);

bad1:
	glexFreeMemory(mesh);

bad0:
	return NULL;
}

GLEX_API void glexDestroyMesh(GLEXMesh *mesh)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);

	if (mesh->opening)
		glexCloseMesh(mesh);

	cx_list_node_unlink(&mesh->node);

	glexFreeMemory(mesh);
}

GLEX_API void glexAddMesh(GLEXMesh *mesh, GLEXMaterial *material)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API unsigned int glexGetMeshSize(GLEXMesh *mesh)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);

	return 0; // mesh->count;
}

GLEX_API unsigned int glexGetMeshVertexCount(GLEXMesh *mesh)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);

	return mesh->count;
}

GLEX_API int glexOpenMesh(GLEXMesh *mesh)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);

	return 0;
}

GLEX_API void glexCloseMesh(GLEXMesh *mesh)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexReadMeshPosition(GLEXMesh *mesh, unsigned int index, float *x, float *y, float *z)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexReadMeshPositionv(GLEXMesh *mesh, unsigned int index, float *v, unsigned int count)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexReadMeshColor(GLEXMesh *mesh, unsigned int index, float *r, float *g, float *b)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexReadMeshColorv(GLEXMesh *mesh, unsigned int index, float *v, unsigned int count)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexReadMeshNormal(GLEXMesh *mesh, unsigned int index, float *x, float *y, float *z)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexReadMeshNormalv(GLEXMesh *mesh, unsigned int index, float *v, unsigned int count)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexReadMeshTexture2D(GLEXMesh *mesh, unsigned int index, float *u, float *v)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexReadMeshTexture2Dv(GLEXMesh *mesh, unsigned int index, float *v, unsigned int count)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexReadMeshIndex(GLEXMesh *mesh, unsigned int offset, unsigned int *index)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexReadMeshIndexv(GLEXMesh *mesh, unsigned int offset, unsigned int *v, unsigned int count)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexWriteMeshPosition(GLEXMesh *mesh, unsigned int index, float x, float y, float z)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexWriteMeshPositionv(GLEXMesh *mesh, unsigned int index, const float *v, unsigned int count)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexWriteMeshColor(GLEXMesh *mesh, unsigned int index, float r, float g, float b)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexWriteMeshColorv(GLEXMesh *mesh, unsigned int index, const float *v, unsigned int count)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexWriteMeshNormal(GLEXMesh *mesh, unsigned int index, float x, float y, float z)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexWriteMeshNormalv(GLEXMesh *mesh, unsigned int index, const float *v, unsigned int count)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexWriteMeshTexture2D(GLEXMesh *mesh, unsigned int index, float u, float v)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexWriteMeshTexture2Dv(GLEXMesh *mesh, unsigned int index, const float *v, unsigned int count)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexWriteMeshIndex(GLEXMesh *mesh, unsigned int offset, unsigned int index)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}

GLEX_API void glexWriteMeshIndexv(GLEXMesh *mesh, unsigned int offset, const unsigned int *v, unsigned int count)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
}
