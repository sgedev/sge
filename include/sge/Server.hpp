//
//
#ifndef SGE_SERVER_HPP
#define SGE_SERVER_HPP

#include <SGE/Common.hpp>
#include <SGE/Game.hpp>

SGE_BEGIN

class Server: public Game {
public:
	Server(uv_loop_t *loop);
	virtual ~Server(void);

public:
	bool start(void) override;
	void stop(void) override;

protected:
	void frame(float elapsed) override;
};

SGE_END

#endif // SGE_SERVER_HPP
