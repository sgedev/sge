//
//
#ifndef SGL_GAME_HPP
#define SGL_GAME_HPP

#include <SGL/Common.hpp>
#include <SGL/Scene.hpp>
#include <SGL/Window.hpp>
#include <SGL/Renderer.hpp>
#include <SGL/Physics.hpp>

SGL_BEGIN

class Game {
public:
	Game(void);
	virtual ~Game(void);

public:
	virtual bool init(void);
	virtual bool handleEvent(const SDL_Event &event);
	virtual void frame(float elapsed);
	Scene &scene(void);

private:
	Scene m_scene;
};

inline Scene &Game::scene(void)
{
	return m_scene;
}

SGL_END

#endif // SGL_GAME_HPP
