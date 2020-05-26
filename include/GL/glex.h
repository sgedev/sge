/*
 *
 */
#ifndef GL_GLEX_H
#define GL_GLEX_H

#include <GL/glew.h>

#ifndef GLEX_API
#	define GLEX_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum {
	GLEX_OK = 0,
	GLEX_ERR_BAD_PARAM
};

typedef enum {
	GLEX_MESH_TYPE_STATIC = 0,
	GLEX_MESH_TYPE_DYNAMIC,
	GLEX_MESH_TYPE_MAX
} GLEXMeshType;

typedef enum {
	GLEX_MESH_MODE_POINTS = 0,
	GLEX_MESH_MODE_LINES,
	GLEX_MESH_MODE_TRIANGLES
} GLEXMeshMode;

typedef struct {
	struct { GLfloat x, y, z; } pos;
	struct { GLfloat x, y, z; } normal;
	struct { GLfloat u, v; } texCoord;
} GLEXVertex;

typedef GLushort GLEXVertexIndex;

typedef struct GLEXContext_ GLEXContext;
typedef struct GLEXLight_ GLEXLight;
typedef struct GLEXMaterial_ GLEXMaterial;
typedef struct GLEXMesh_ GLEXMesh;

GLEX_API GLEXContext *glexCreateContext(void);
GLEX_API GLEXContext *glexCurrentContext(void);
GLEX_API void glexDeleteContext(GLEXContext *context);
GLEX_API void glexMakeCurrent(GLEXContext *context);

GLEX_API void glexBeginFrame(void);
GLEX_API void glexEndFrame(void);
GLEX_API void glexViewport(int x, int y, int width, int height);
GLEX_API void glexViewMatrix(const float *matrix4x4);
GLEX_API void glexProjectionMatrix(const float *matrix4x4);

GLEX_API GLEXLight *glexCreateLight(void);
GLEX_API void glexDeleteLight(GLEXLight *light);
GLEX_API void glexAddLight(GLEXLight *lightconst, const float *transform);

GLEX_API GLEXMaterial *glexCreateMaterial(void);
GLEX_API void glexDeleteMaterial(GLEXMaterial *material);

GLEX_API GLEXMesh *glexCreateMesh(GLEXMeshType type, GLEXMeshMode mode, int vertexCount, int vertexIndexCount);
GLEX_API void glexDeleteMesh(GLEXMesh *mesh);
GLEX_API void glexMeshVertexData(GLEXMesh *mesh, int offset, const GLEXVertex *p, int count);
GLEX_API void glexMeshVertexIndexData(GLEXMesh *mesh, int offset, const GLEXVertexIndex *p, int count);
GLEX_API void glexAddMesh(GLEXMesh *mesh, GLEXMaterial *material, const float *transform);

#ifdef __cplusplus
}
#endif

#endif /* GL_GLEX_H */
