//
//
#include <SGE/Server.hpp>

SGE_BEGIN

Server::Server(uv_loop_t *loop):
	Handle(loop)
{
}

Server::~Server(void)
{
}

bool Server::start(const std::string &path)
{
	if (!Handle::start(path))
		return false;

	return true;
}

void Server::stop(void)
{
	Handle::stop();
}

SGE_END
