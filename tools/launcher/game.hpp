//
//
#ifndef SGE_LAUNCHER_GAME_HPP
#define SGE_LAUNCHER_GAME_HPP

#include <SDL.h>

#include <sge/game.hpp>

class Game: public SGE::Game {
public:
	Game(void);
	virtual ~Game(void);

public:
	bool init(const char *root);
	void shutdown(void) override;
	bool handleEvent(const SDL_Event *event);
	void frame(float elapsed);

protected:
	int trapFps(lua_State *L) override;
	int trapEditorIsEnabled(lua_State *L) override;

private:
	bool handleKeyEvent(const SDL_Event *event);
	bool handleMouseButtonEvent(const SDL_Event *event);
	bool handleMouseMoveEvent(const SDL_Event *event);
	bool handleWindowEvent(const SDL_Event *event);

private:
	SDL_Window *m_window;
	Uint32 m_windowId;
	int m_windowRect[4];
	bool m_windowVisibled;
	SDL_GLContext m_gl;
	union GL3WProcs m_gl3w;
	SGE::View m_view;
};

#endif // SGE_LAUNCHER_GAME_HPP

