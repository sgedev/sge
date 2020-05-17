//
//
#include <cstdio>

#include <SGE/Game.hpp>

SGE_BEGIN

Game::Game(uv_loop_t *loop):
	m_loop(loop),
	m_state(StateIdle)
{
	SGE_ASSERT(loop != NULL);

	uv_timer_init(loop, &m_frameTimer);
	m_frameTimer.data = this;

	uv_timer_init(loop, &m_stateTimer);
	m_stateTimer.data = this;

	char buf[128];
	snprintf(buf, sizeof(buf), "/sge/game/%llx", (uintptr_t)this);
	m_root = buf;
}

Game::~Game(void)
{
	if (m_state != StateIdle)
		stop();
}

bool Game::start(const std::string &path)
{
	SGE_ASSERT(m_state == StateIdle);

	if (!m_scene.init())
		return false;

	if (!m_physicsWorld.init())
		return false;

	m_scene.addListener(&m_physicsWorld);

	uv_timer_start(&m_frameTimer, &Game::frameCallback, 16, 16);
	uv_timer_start(&m_stateTimer, &Game::stateCallback, 1000, 1000);

	m_fps = 0;
	m_fpsCount = 0;
	m_last = SDL_GetTicks();
	m_state = StateLoading;

	return true;
}

void Game::stop(void)
{
	SGE_ASSERT(m_state != StateIdle);

	uv_timer_stop(&m_stateTimer);
	uv_timer_stop(&m_frameTimer);

	m_state = StateIdle;
}

bool Game::handleEvent(const SDL_Event &event)
{
	return false;
}

void Game::doLoading(void)
{
	updateLoading(0.1f);
}

void Game::doPlaying(void)
{
	m_scene.update(m_elapsed);
	m_physicsWorld.update(m_elapsed);

	updatePlaying();
}

void Game::doPaused(void)
{

}

void Game::doError(void)
{

}

void Game::frame(void)
{
	Uint32 curr = SDL_GetTicks() - m_last;
	m_elapsed = float(curr) / 1000.0f;

	switch (m_state) {
	case StateLoading:
		doLoading();
		break;
	case StatePlaying:
		doPlaying();
		break;
	case StatePaused:
		doPaused();
		break;
	case StateError:
		doError();
		break;
	default:
		SGE_ASSERT(false);
		break;
	}

	m_fpsCount += 1;
	uv_update_time(m_loop);
}

void Game::frameCallback(uv_timer_t *p)
{
	((Game *)(p->data))->frame();
}

void Game::state(void)
{
	m_fps = m_fpsCount;
	m_fpsCount = 0;
}

void Game::stateCallback(uv_timer_t *p)
{
	((Game *)(p->data))->state();
}

SGE_END
