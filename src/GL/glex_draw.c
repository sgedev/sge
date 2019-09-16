/*
 *
 */
#include "glex_common.h"

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
}

GLEX_API void glexEndFrame(void)
{
	GLEX_ASSERT(glex != NULL);
}

GLEX_API void glexRender(void)
{
	GLEX_ASSERT(glex != NULL);

	glViewport(glex->viewport[0], glex->viewport[1], glex->viewport[2], glex->viewport[3]);

	glUseProgram(glex->program);

	glexCommitUniforms();

	glexDrawTest();
}
