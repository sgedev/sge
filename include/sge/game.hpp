//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <sge/common.hpp>
#include <sge/noncopyable.hpp>
#include <sge/console.hpp>

SGE_BEGIN

class game: public noncopyable {
public:
	game(void);
	virtual ~game(void);

public:
	bool init(void);
	void shutdown(void);
	void frame(float elapsed);
	void handle_event(const SDL_Event &event);

protected:
	virtual void update(float elapsed);
	virtual void draw(void);

private:
	console m_console;
};

SGE_END

#endif // SGE_GAME_HPP
