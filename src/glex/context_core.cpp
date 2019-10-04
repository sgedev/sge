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

bool Context::init(void)
{
	if (!initProgram())
		goto bad0;

	if (!initTest())
		goto bad1;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return true;

bad1:
	shutdownProgram();

bad0:
	return false;
}

void Context::shutdown(void)
{
	shutdownTest();
	shutdownProgram();
}

GLEX_END

