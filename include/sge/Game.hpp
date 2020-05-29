//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <string>

#include <SGE/Common.hpp>
#include <SGE/VM.hpp>
#include <SGE/Scene.hpp>
#include <SGE/Physics.hpp>

SGE_BEGIN

class Game: public VM::Kernel {
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
	virtual bool handleEvent(const SDL_Event &event);

protected:
	bool init(void) override;
	void shutdown(void) override;
	void barrier(void);
	void updateAsync(void);
	virtual void frame(float elapsed);
	virtual void update(void);

private:
	static void frameCallback(uv_timer_t *p);
	static void updateCallback(uv_async_t *p);

private:
	std::string m_root;
	State m_state;
	uv_timer_t m_frameTimer;
	uv_async_t m_updateAsync;
	uv_barrier_t m_barrier;
	uint64_t m_last;
	Physics::World m_physicsWorld;
};

SGE_INLINE void Game::barrier(void)
{
	uv_barrier_wait(&m_barrier);
}

SGE_INLINE void Game::updateAsync(void)
{
	uv_async_send(&m_updateAsync);
}

SGE_END

#endif // SGE_GAME_HPP
