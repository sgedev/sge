//
//
#include <GLEX/context.hpp>

GLEX_BEGIN

void Context::BeginFrame(void)
{
}

void Context::EndFrame(void)
{
	glUseProgram(m_program);

	CommitUniforms();

	DrawTest();
}

GLEX_END

