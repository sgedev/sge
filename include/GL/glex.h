/*
 *
 */
#ifndef GLEX_H
#define GLEX_H

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
	GLEX_MESH_MODE_TRIAGLEXES
} GLEXMeshMode;

typedef enum {
	GLEX_RENDER_MODE_NONE = 0,
	GLEX_RENDER_MODE_FORWARD,
	GLEX_RENDER_MODE_FORWARD_PLUS,
	GLEX_RENDER_MODE_DEFERRED,
} GLEXRenderMode;

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

GLEX_API void glexRenderMode(GLEXRenderMode mode);

GLEX_API void glexBeginFrame(void);
GLEX_API void glexEndFrame(void);
GLEX_API void glexViewMatrix(const GLfloat *matrix4x4);
GLEX_API void glexProjectionMatrix(const GLfloat *matrix4x4);

GLEX_API GLEXLight *glexCreateLight(void);
GLEX_API void glexDeleteLight(GLEXLight *light);
GLEX_API void glexAddLight(GLEXLight *light, const GLfloat *transform);

GLEX_API GLEXMaterial *glexCreateMaterial(void);
GLEX_API void glexDeleteMaterial(GLEXMaterial *material);

GLEX_API GLEXMesh *glexCreateMesh(GLEXMeshType type, GLEXMeshMode mode, GLint vertexCount, int vertexIndexCount);
GLEX_API void glexDeleteMesh(GLEXMesh *mesh);
GLEX_API void glexMeshVertexData(GLEXMesh *mesh, GLint offset, const GLEXVertex *p, GLint count);
GLEX_API void glexMeshVertexIndexData(GLEXMesh *mesh, GLint offset, const GLEXVertexIndex *p, GLint count);
GLEX_API void glexAddMesh(GLEXMesh *mesh, GLEXMaterial *material, const GLfloat *transform);

#ifdef __cplusplus
}
#endif

#endif /* GLEX_H */

