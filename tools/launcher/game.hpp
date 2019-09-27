//
//
#ifndef SGE_LAUNCHER_GAME_HPP
#define SGE_LAUNCHER_GAME_HPP

#include <SDL.h>

#include <sge/vm.hpp>

class Game {
public:
	Game(void);
	virtual ~Game(void);

public:
	bool Init(void);
	void Shutdown(void);
	bool HandleEvent(const SDL_Event *event);
	void Frame(float elapsed);

private:

private:
	SDL_Window *m_window;
	Uint32 m_window_id;
	int m_window_rect[4];
	SDL_GLContext m_gl;
	union GL3WProcs m_gl3w;
	GLEX::Context m_glex;
	sge::vm m_vm;
	sge::view m_view;
};

#endif // SGE_LAUNCHER_GAME_HPP

