//
//
#ifndef SGE_SERVER_HPP
#define SGE_SERVER_HPP

#include <sge/subsystem.hpp>
#include <sge/game.hpp>

SGE_BEGIN

class server: public subsystem {
public:
	server(uv_loop_t *loop = NULL);
	virtual ~server(void);

protected:
	bool init(void) override;
	void shutdown(void) override;
	bool handle_event(const SDL_Event *event) override;

private:
	game m_game;
};

SGE_END

#endif // SGE_SERVER_HPP

