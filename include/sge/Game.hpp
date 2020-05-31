//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <list>
#include <string>

#include <cx/signals.hpp>

#include <SGE/Common.hpp>
#include <SGE/Object.hpp>
#include <SGE/Camera.hpp>
#include <SGE/Scene.hpp>
#include <SGE/Physics.hpp>

SGE_BEGIN

typedef std::list<ObjectPtr> ObjectList;
typedef std::list<CameraPtr> CameraList;

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
	virtual bool start(void);
	virtual void stop(void);
	virtual void frame(float elapsed);
	virtual bool handleEvent(const SDL_Event &event);
	void setName(const std::string &name);
	void setGravity(const glm::vec3 &v);
	Scene &scene(void);
	State state(void) const;
	int fps(void) const;
	int fpsMax(void) const;
	void setFpsMax(int v);
	float elapsed(void) const;

public:
	cx::signal<void(const std::string &)> nameChanged;
	cx::signal<void(const glm::vec3 &)> gravityChanged;

private:
	static void frameCallback(uv_timer_t *p);
	static void stateCallback(uv_timer_t *p);

private:
	uv_loop_t *m_loop;
	std::string m_name;
	Scene m_scene;
	ObjectList m_objectList;
	CameraList m_cameraList;
	State m_state;
	uv_timer_t m_frameTimer;
	uv_timer_t m_stateTimer;
	uint64_t m_last;
	int m_fps;
	int m_fpsCount;
	float m_elapsed;
	float m_elapsedMin;
	Physics::World m_physicsWorld;
};

SGE_INLINE Scene &Game::scene(void)
{
	return m_scene;
}

SGE_INLINE Game::State Game::state(void) const
{
	return m_state;
}

SGE_INLINE int Game::fps(void) const
{
	return m_fps;
}

SGE_INLINE int Game::fpsMax(void) const
{
	return int(1000.0f / m_elapsedMin);
}

SGE_INLINE float Game::elapsed(void) const
{
	return m_elapsed;
}

SGE_END

#endif // SGE_GAME_HPP
