//
//
#include <GLEX/context.hpp>

GLEX_BEGIN

Context::Context(void)
{
}

Context::~Context(void)
{
}

bool Context::Init(void)
{
	if (!InitProgram())
		goto bad0;

	if (!InitTest())
		goto bad1;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return true;

bad1:
	ShutdownProgram();

bad0:
	return false;
}

void Context::Shutdown(void)
{
	ShutdownTest();
	ShutdownProgram();
}

GLEX_END

