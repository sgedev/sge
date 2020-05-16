//
//
#ifndef SGE_SERVER_HPP
#define SGE_SERVER_HPP

#include <SGE/Common.hpp>
#include <SGE/Handle.hpp>

SGE_BEGIN

class Server: public Handle {
public:
	Server(uv_loop_t *loop);
	virtual ~Server(void);

public:
	bool start(const std::string &path) override;
	void stop(void) override;
};

SGE_END

#endif // SGE_SERVER_HPP
