//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <string>

#include <SGE/Common.hpp>
#include <SGE/Scene.hpp>
#include <SGE/Physics.hpp>

SGE_BEGIN

class Game {
public:
	enum State {
		StateIdle = 0,
		StateLoading,
		StatePlaying,
		StatePaused,
		StateError
	};

public:
	Game(uv_loop_t *loop);
	virtual ~Game(void);

public:
	virtual bool start(const std::string &path);
	virtual void stop(void);
	virtual bool handleEvent(const SDL_Event &event);

protected:
	float elapsed(void) const;
	virtual void updateLoading(float progress) = 0;
	virtual void updatePlaying(void) = 0;

private:
	void doLoading(void);
	void doPlaying(void);
	void doPaused(void);
	void doError(void);
	void frame(void);
	static void frameCallback(uv_timer_t *p);
	void state(void);
	static void stateCallback(uv_timer_t *p);

protected:
	Scene m_scene;

private:
	uv_loop_t *m_loop;
	uv_timer_t m_frameTimer;
	uv_timer_t m_stateTimer;
	State m_state;
	int m_fps;
	int m_fpsCount;
	Uint32 m_last;
	float m_elapsed;
	Physics::World m_physicsWorld;
	std::string m_root;
};

SGE_INLINE float Game::elapsed(void) const
{
	return m_elapsed;
}

SGE_END

#endif // SGE_GAME_HPP
