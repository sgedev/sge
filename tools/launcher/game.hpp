//
//
#ifndef SGE_LAUNCHER_GAME_HPP
#define SGE_LAUNCHER_GAME_HPP

#include <SDL.h>

#include <sge/game.hpp>

#include "filesystem.hpp"

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
	bool HandleKeyEvent(const SDL_Event* event);
	bool HandleMouseButtonEvent(const SDL_Event* event);
	bool HandleMouseMoveEvent(const SDL_Event* event);
	bool HandleWindowEvent(const SDL_Event* event);

private:
	SDL_Window *m_window;
	Uint32 m_window_id;
	int m_window_rect[4];
	bool m_window_visibled;
	SDL_GLContext m_gl;
	union GL3WProcs m_gl3w;
	GLEX::Context m_glex;
	Filesystem m_fs;
	sge::game m_game;
	sge::view m_view;
};

#endif // SGE_LAUNCHER_GAME_HPP

