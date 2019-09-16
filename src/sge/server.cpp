//
//
#include <sge/server.hpp>

SGE_BEGIN

server::server(uv_loop_t *loop)
	: subsystem(loop)
{
}

server::~server(void)
{
}

bool server::init(void)
{
	return false;
}

void server::shutdown(void)
{
}

bool server::handle_event(const SDL_Event *event)
{
	return false;
}

SGE_END

