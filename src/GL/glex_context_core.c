//
//
#include <string.h>

#include "glex_context.h"

GLEXContext *glex;

GLEX_API GLEXContext *glexCreateContext(GL3WGetProcAddressProc proc)
{
	GLEXContext *context;
	union GL3WProcs *backup = gl3wProcs;

	if (proc == NULL)
		goto bad0;

	context = glexAllocMemory(sizeof(GLEXContext));
	if (context == NULL)
		goto bad0;

	memset(context, 0, sizeof(GLEXContext));

	if (gl3wInit(&context->gl3w, proc) < 0)
		goto bad1;

	gl3wProcs = &context->gl3w;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (!glexInitProgram(context))
		goto bad1;

	if (!glexInitTest(context))
		goto bad2;

	gl3wProcs = backup;

	return context;

bad2:
	glexShutdownTest(context);

bad1:
	gl3wProcs = backup;
	glexFreeMemory(context);

bad0:
	return NULL;
}

GLEX_API void glexDeleteContext(GLEXContext *context)
{
	union GL3WProcs *backup;

	if (context == NULL)
		return;

	if (glex == context)
		glex = NULL;

	backup = gl3wProcs;
	gl3wProcs = &context->gl3w;

	glexShutdownTest(context);
	glexShutdownProgram(context);

	glexFreeMemory(context);

	gl3wProcs = backup;
}

GLEX_API GLEXContext *glexGetCurrentContext(void)
{
	return glex;
}

GLEX_API void glexMakeCurrent(GLEXContext *context)
{
	glex = context;
}

GLEX_API void glexElapsed(GLfloat v)
{
	GLEX_ASSERT(glex != NULL);
	glex->uniformFloat[GLEX_UNIFORM_ELAPSED] = v;
}

GLEX_API void glexModelMatrix(const GLfloat *v)
{
	GLEX_ASSERT(glex != NULL);
	memcpy(glex->uniformMat4[GLEX_UNIFORM_MODEL_MATRIX].Elements, v, sizeof(GLfloat) * 16);
}

GLEX_API void glexViewMatrix(const GLfloat *v)
{
	GLEX_ASSERT(glex != NULL);
	memcpy(glex->uniformMat4[GLEX_UNIFORM_VIEW_MATRIX].Elements, v, sizeof(GLfloat) * 16);
}

GLEX_API void glexProjectionMatrix(const GLfloat *v)
{
	GLEX_ASSERT(glex != NULL);
	memcpy(glex->uniformMat4[GLEX_UNIFORM_PROJECTION_MATRIX].Elements, v, sizeof(GLfloat) * 16);
}

void glexInitContextGlobals(void)
{
	glex = NULL;
}

