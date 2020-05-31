//
//
#include <SGE/Server.hpp>

SGE_BEGIN

Server::Server(uv_loop_t *loop):
	Game(loop)
{
}

Server::~Server(void)
{
}

bool Server::start(void)
{
	if (!Game::start())
		return false;

	return true;
}

void Server::stop(void)
{
	Game::stop();
}

void Server::frame(float elapsed)
{
}

SGE_END
