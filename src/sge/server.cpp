//
//
#include <sge/server.hpp>

SGE_BEGIN

server::server(uv_loop_t *lp)
	: subsystem(lp)
{
}

server::~server(void)
{
}

bool server::init(void)
{
	return true;
}

void server::shutdown(void)
{
}

SGE_END

