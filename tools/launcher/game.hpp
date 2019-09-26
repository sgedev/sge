//
//
#ifndef SGE_LAUNCHER_GAME_HPP
#define SGE_LAUNCHER_GAME_HPP

#include <SDL.h>

#include <sge/game.hpp>

class Game {
public:
	Game(void);
	virtual ~Game(void);

public:
	bool Init(void);
	void Shutdown(void);
	void Frame(float elapsed);

private:
	SDL_Window *m_window;
	Uint32 m_window_id;
	int m_window_rect[4];
	SDL_GLContext m_gl;
	union GL3WProcs m_gl3w;
	GLEX::Context m_glex;
	sge::game m_game;
	sge::view m_view;
};

#endif // SGE_LAUNCHER_GAME_HPP

