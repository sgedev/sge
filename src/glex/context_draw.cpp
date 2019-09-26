//
//
#include <GLEX/context.hpp>

GLEX_BEGIN

void Context::BeginFrame(void)
{
}

void Context::EndFrame(void)
{
	CommitUniforms();

	DrawTest();
}

GLEX_END

