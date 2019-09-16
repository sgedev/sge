/*
 *
 */
#ifndef GLEX_H
#define GLEX_H

#include <stdarg.h>

#include <GL/glcorearb.h>

#ifdef __cplusplus
#	define GLEX_EXTEN_C extern "C"
#	define GLEX_BEGIN_DECLS extern "C" {
#	define GLEX_END_DECLS }
#else
#	define GLEX_EXTEN_C
#	define GLEX_BEGIN_DECLS
#	define GLEX_END_DECLS
#endif

#if defined(_MSC_VER)
#	define GLEX_DLLEXPORT __declspec(dllexport)
#	define GLEX_DLLIMPORT __declspec(dllimport)
#elif defined(__GNUC__)
#	define GLEX_DLLEXPORT
#	define GLEX_DLLIMPORT
#else
#	error unsupported compiler.
#endif

#ifdef GLEX_SHARED
#	ifdef GLEX
#		define GLEX_API GLEX_DLLEXPORT
#	else
#		define GLEX_API GLEX_DLLIMPORT
#	endif
#else
#	define GLEX_API
#endif

GLEX_BEGIN_DECLS

typedef enum {
	GLEX_LOG_TYPE_ERROR,
	GLEX_LOG_TYPE_WARNING,
	GLEX_LOG_TYPE_INFO,
	GLEX_LOG_TYPE_DEBUG
} GLEXLogType;

typedef struct {
	void (* LogPrint)(GLEXLogType type, const char *fmt, va_list args);
	void (* FatalError)(const char *fmt, va_list args);
	void *(* AllocMemory)(unsigned int size);
	void (* FreeMemory)(void *ptr);
} GLEXSystem;

typedef struct {
	unsigned int static_buffer_size;
	unsigned int dynamic_buffer_size;
} GLEXConfig;

enum {
	GLEX_MESH_FLAG_RGB = 0x1,
	GLEX_MESH_FLAG_NORMAL = 0x2,
	GLEX_MESH_FLAG_UV = 0x4,
	GLEX_MESH_FLAG_INDEXED = 0x8,
	GLEX_MESH_FLAG_DYNAMIC = 0x10
};

typedef struct {
	unsigned int flags;
	unsigned int count;
} GLEXMeshInfo;

typedef enum {
	GLEX_MATERIAL_TYPE_NONE = 0,
	GLEX_MATERIAL_TYPE_PHONG,
	GLEX_MATERIAL_TYPE_BLINN_PHONG
} GLEXMaterialType;

typedef struct {
	GLEXMaterialType type;
} GLEXMaterialInfo;

typedef enum {
	GLEX_LIGHT_TYPE_SPOT = 0,
} GLEXLightType;

typedef struct GLEXContext_ GLEXContext;
typedef struct GLEXMaterial_ GLEXMaterial;
typedef struct GLEXMesh_ GLEXMesh;
typedef struct GLEXLight_ GLEXLight;

GLEX_API int glexInit(const GLEXSystem *system);
GLEX_API void glexShutdown(void);

GLEX_API GLEXContext *glexCreateContext(const GLEXConfig *config);
GLEX_API void glexDeleteContext(GLEXContext *context);
GLEX_API GLEXContext *glexGetCurrentContext(void);
GLEX_API void glexMakeCurrent(GLEXContext *context);

GLEX_API void glexViewport(int x, int y, int width, int height);
GLEX_API void glexClear(void);
GLEX_API void glexRender(void);
GLEX_API void glexModelMatrix(float *m);
GLEX_API void glexViewMatrix(float *m);
GLEX_API void glexProjectionMatrix(float *m);

GLEX_API GLEXMaterial *glexCreateMaterial(const GLEXMaterialInfo *info);
GLEX_API void glexDestroyMaterial(GLEXMaterial *material);
GLEX_API GLEXMaterialType glexGetMaterialType(GLEXMaterial *material);
GLEX_API void glexMaterialDiffuse(GLEXMaterial *material, float red, float green, float blue);
GLEX_API void glexMaterialSpecular(GLEXMaterial *material, float red, float green, float blue);
//GLEX_API void glexMaterialPhong(GLEXMaterial *material, 

GLEX_API GLEXLight *glexCreateLight(GLEXLightType type);
GLEX_API void glexDestroyLight(GLEXLight *light);
GLEX_API void glexAddLight(GLEXLight *light);

GLEX_API GLEXMesh *glexCreateMesh(const GLEXMeshInfo *info);
GLEX_API void glexDestroyMesh(GLEXMesh *mesh);
GLEX_API void glexAddMesh(GLEXMesh *mesh, GLEXMaterial *material);
GLEX_API unsigned int glexGetMeshSize(GLEXMesh *mesh);
GLEX_API unsigned int glexGetMeshVertexCount(GLEXMesh *mesh);

GLEX_API int glexOpenMesh(GLEXMesh *mesh);
GLEX_API void glexCloseMesh(GLEXMesh *mesh);
GLEX_API void glexReadMeshPosition(GLEXMesh *mesh, unsigned int offset, float *x, float *y, float *z);
GLEX_API void glexReadMeshPositionv(GLEXMesh *mesh, unsigned int offset, float *v, unsigned int count);
GLEX_API void glexReadMeshColor(GLEXMesh *mesh, unsigned int offset, float *r, float *g, float *b);
GLEX_API void glexReadMeshColorv(GLEXMesh *mesh, unsigned int offset, float *v, unsigned int count);
GLEX_API void glexReadMeshNormal(GLEXMesh *mesh, unsigned int offset, float *x, float *y, float *z);
GLEX_API void glexReadMeshNormalv(GLEXMesh *mesh, unsigned int offset, float *v, unsigned int count);
GLEX_API void glexReadMeshTexture2D(GLEXMesh *mesh, unsigned int offset, float *u, float *v);
GLEX_API void glexReadMeshTexture2Dv(GLEXMesh *mesh, unsigned int offset, float *v, unsigned int count);
GLEX_API void glexReadMeshIndex(GLEXMesh *mesh, unsigned int offset, unsigned int *index);
GLEX_API void glexReadMeshIndexv(GLEXMesh *mesh, unsigned int offset, unsigned int *v, unsigned int count);
GLEX_API void glexWriteMeshPosition(GLEXMesh *mesh, unsigned int offset, float x, float y, float z);
GLEX_API void glexWriteMeshPositionv(GLEXMesh *mesh, unsigned int offset, const float *v, unsigned int count);
GLEX_API void glexWriteMeshColor(GLEXMesh *mesh, unsigned int offset, float r, float g, float b);
GLEX_API void glexWriteMeshColorv(GLEXMesh *mesh, unsigned int offset, const float *v, unsigned int count);
GLEX_API void glexWriteMeshNormal(GLEXMesh *mesh, unsigned int offset, float x, float y, float z);
GLEX_API void glexWriteMeshNormalv(GLEXMesh *mesh, unsigned int offset, const float *v, unsigned int count);
GLEX_API void glexWriteMeshTexture2D(GLEXMesh *mesh, unsigned int offset, float u, float v);
GLEX_API void glexWriteMeshTexture2Dv(GLEXMesh *mesh, unsigned int offset, const float *v, unsigned int count);
GLEX_API void glexWriteMeshIndex(GLEXMesh *mesh, unsigned int offset, unsigned int index);
GLEX_API void glexWriteMeshIndexv(GLEXMesh *mesh, unsigned int offset, const unsigned int *v, unsigned int count);

GLEX_END_DECLS

#endif /* GLEX_H */

