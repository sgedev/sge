//
//
#include <GLEX/context.hpp>

GLEX_BEGIN

void Context::beginFrame(void)
{
}

void Context::endFrame(void)
{
	glUseProgram(m_program);

	commitUniforms();

	drawTest();
}

GLEX_END

