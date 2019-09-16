/*
 *
 */
#ifndef GLEX_COMMON_H
#define GLEX_COMMON_H

#include <cx/list.h>

#include <GLXW/glxw.h>
#include <GLEX/glex.h>

#include <HandmadeMath.h>

#define GLEX_ASSERT CX_ASSERT
#define GLEX_INLINE inline

struct GLEXLight_ {
	cx_list_node_t node;
	GLEXLightType type;
};

struct GLEXMaterial_ {
	cx_list_node_t node;
};

struct GLEXMesh_ {
	unsigned int flags;
	int first_vertex;
	int first_element;
	unsigned int count;
	GLboolean opening;
	cx_list_node_t node;
};

typedef enum {
	GLEX_UNIFORM_ELAPSED = 0,
	GLEX_UNIFORM_FLOAT_MAX
} GELXUniformFloat;

typedef enum {
	GLEX_UNIFORM_MODEL_MATRIX = 0,
	GLEX_UNIFORM_VIEW_MATRIX,
	GLEX_UNIFORM_PROJECTION_MATRIX,
	GLEX_UNIFORM_MAT4_MAX
} GELXUniformMat4;

struct GLEXContext_ {
	struct glxw glxw_ctx;

	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;
	GLint uniform_float_loc[GLEX_UNIFORM_FLOAT_MAX];
	GLfloat uniform_float[GLEX_UNIFORM_FLOAT_MAX];
	GLint uniform_mat4_loc[GLEX_UNIFORM_MAT4_MAX];
	hmm_mat4 uniform_mat4[GLEX_UNIFORM_MAT4_MAX];

	cx_list_t mesh_list;
	GLuint static_vertex_buffer;
	GLuint static_index_buffer;
	GLuint dynamic_vertex_buffer;
	GLuint dynamic_index_buffer;

	GLuint test_vertex_buffer;
	GLuint test_index_buffer;
	GLuint test_vertex_array;
};

extern GLEXContext *glex;

void glexFatalError(const char *fmt, ...);
void glexLogPrint(GLEXLogType type, const char *fmt, va_list args);
void *glexAllocMemory(unsigned int size);
void glexFreeMemory(void *ptr);
GLboolean glexInitLight(GLEXContext *context);
void glexShutdownLight(GLEXContext *context);
GLboolean glexInitMaterial(GLEXContext *context);
void glexShutdownMaterial(GLEXContext *context);
GLboolean glexInitMesh(GLEXContext *context);
void glexShutdownMesh(GLEXContext *context);
GLboolean glexInitProgram(GLEXContext *context);
void glexShutdownProgram(GLEXContext *context);
void glexCommitUniforms(void);
GLboolean glexInitTest(GLEXContext *context);
void glexShutdownTest(GLEXContext *context);
void glexDrawTest(void);

static GLEX_INLINE void glexLogError(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	glexLogPrint(GLEX_LOG_TYPE_ERROR, fmt, args);
	va_end(args);
}

static GLEX_INLINE void glexLogWarning(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	glexLogPrint(GLEX_LOG_TYPE_WARNING, fmt, args);
	va_end(args);
}

static GLEX_INLINE void glexLogInfo(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	glexLogPrint(GLEX_LOG_TYPE_INFO, fmt, args);
	va_end(args);
}

static GLEX_INLINE void glexLogDebug(const char *fmt, ...)
{
#ifdef GLEX_DEBUG
	va_list args;

	va_start(args, fmt);
	glexLogPrint(GLEX_LOG_TYPE_DEBUG, fmt, args);
	va_end(args);
#endif
}

#endif /* GLEX_COMMON_H */

