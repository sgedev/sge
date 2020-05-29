//
//
#include <cstdio>

#include <SGE/Game.hpp>

SGE_BEGIN

Game::Game(uv_loop_t *loop):
	Kernel(loop),
	m_state(StateIdle)
{
	SGE_ASSERT(loop != NULL);

	uv_async_init(loop, &m_updateAsync, &Game::updateCallback);
	m_updateAsync.data = this;
}

Game::~Game(void)
{
	if (m_state != StateIdle)
		stop();
}

bool Game::handleEvent(const SDL_Event &event)
{
	// TODO
	return false;
}

bool Game::init(void)
{
	uv_barrier_init(&m_barrier, 2);

	if (!m_physicsWorld.init())
		return false;

	uv_timer_init(loop(), &m_frameTimer);
	uv_timer_start(&m_frameTimer, &Game::frameCallback, 0, 16);
	m_frameTimer.data = this;

	m_last = uv_now(loop());

	return true;
}

void Game::shutdown(void)
{
	SGE_ASSERT(m_state != StateIdle);

	uv_timer_stop(&m_frameTimer);

	uv_barrier_destroy(&m_barrier);

	m_state = StateIdle;
}

void Game::frame(float elapsed)
{
	SGE_ASSERT(inKernel());

	m_physicsWorld.update(elapsed);
}

void Game::frameCallback(uv_timer_t *p)
{
	Game *G = ((Game *)(p->data));

	uint64_t curr = uv_now(p->loop);
	uint64_t pass = curr - G->m_last;

	G->frame(float(pass) / 1000.0f);
	G->m_last = curr;

	uv_update_time(p->loop);
}

void Game::update(void)
{
	SGE_ASSERT(!inKernel());
}

void Game::updateCallback(uv_async_t *p)
{
	Game *G = ((Game *)(p->data));
	G->update();
}

SGE_END
