/*
 *
 */
#ifndef GLEX_CONTEXT_H
#define GLEX_CONTEXT_H

#include "glex_common.h"
#include "glex_mesh.h"
#include "glex_material.h"

#define GLEX_MAX_OBJECTS 512

typedef enum {
	GLEX_UNIFORM_ELAPSED = 0,
	GLEX_UNIFORM_FLOAT_MAX
} GLEXUniformFloat;

typedef enum {
	GLEX_UNIFORM_MODEL_MATRIX = 0,
	GLEX_UNIFORM_VIEW_MATRIX,
	GLEX_UNIFORM_PROJECTION_MATRIX,
	GLEX_UNIFORM_MAT4_MAX
} GLEXUniformMat4;

struct GLEXContext_ {
	union GL3WProcs gl3w;
	union GL3WProcs *gl3wBackup;

	int viewport[4];

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;
	GLint uniformFloatLoc[GLEX_UNIFORM_FLOAT_MAX];
	GLfloat uniformFloat[GLEX_UNIFORM_FLOAT_MAX];
	GLint uniformMat4Loc[GLEX_UNIFORM_MAT4_MAX];
	hmm_mat4 uniformMat4[GLEX_UNIFORM_MAT4_MAX];

	GLuint testVertexBuffer;
	GLuint testIndexBuffer;
	GLuint testVertexArray;

	void *objectTable[GLEX_MAX_OBJECTS];
};

extern GLEXContext *glex;

void glexInitContextGlobals(void);
GLboolean glexInitProgram(GLEXContext *context);
void glexShutdownProgram(GLEXContext *context);
void glexCommitUniforms(void);
GLboolean glexInitTest(GLEXContext *context);
void glexShutdownTest(GLEXContext *context);
void glexDrawTest(void);

#endif /* GLEX_CONTEXT_H */

