/*
 *
 */
#include "glex_context.h"

GLEX_API void glexViewport(int x, int y, int width, int height)
{
	GLEX_ASSERT(glex != NULL);

	glex->viewport[0] = x;
	glex->viewport[1] = y;
	glex->viewport[2] = width;
	glex->viewport[3] = height;
}

GLEX_API void glexBeginFrame(void)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(glex->gl3wBackup == NULL);

	glex->gl3wBackup = gl3wProcs;
	gl3wProcs = &glex->gl3w;

	glViewport(glex->viewport[0], glex->viewport[1], glex->viewport[2], glex->viewport[3]);
}

GLEX_API void glexEndFrame(void)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(gl3wProcs == &glex->gl3w);

	glUseProgram(glex->program);

	glexCommitUniforms();

	glexDrawTest();

	gl3wProcs = glex->gl3wBackup;
	glex->gl3wBackup = NULL;
}

