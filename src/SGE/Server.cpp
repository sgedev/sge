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

bool Server::start(const std::string &path)
{
	if (!Game::start(path))
		return false;

	return true;
}

void Server::stop(void)
{
	Game::stop();
}

void Server::updateLoading(float progress)
{
}

void Server::updatePlaying(void)
{
}

SGE_END
