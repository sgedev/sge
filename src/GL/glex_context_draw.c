/*
 *
 */
#include "glex_context.h"

GLEX_API void glexBeginFrame(void)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(glex->gl3wBackup == NULL);

	glex->gl3wBackup = gl3wProcs;
	gl3wProcs = &glex->gl3w;
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

