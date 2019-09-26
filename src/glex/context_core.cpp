//
//
#include <GLEX/context.hpp>

GLEX_BEGIN

Context::Context(void)
{
}

Context::~Context(void)
{
	ShutdownTest();
	ShutdownProgram();
}

bool Context::Init(void)
{
	if (!InitProgram()) {
		goto bad0;
	}

	if (!InitTest()) {
		goto bad1;
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return true;

bad1:
	ShutdownProgram();

bad0:
	return false;
}

GLEX_END

