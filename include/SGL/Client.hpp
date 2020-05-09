//
//
#ifndef SGL_CLIENT_HPP
#define SGL_CLIENT_HPP

#include <SGL/Common.hpp>
#include <SGL/Game.hpp>

SGL_BEGIN

class Client: public Game {
public:
	Client(void);
	virtual ~Client(void);

public:
	virtual bool init(const char *name);
	Window *window(void);
	Renderer *renderer(void);

protected:
	virtual Window *getWindow(void) = 0;
	virtual Renderer *getRenderer(void) = 0;

private:
};

SGL_END

#endif // SGL_CLIENT_HPP

