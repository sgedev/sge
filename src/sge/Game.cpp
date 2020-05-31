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
}

Game::~Game(void)
{
	if (m_state != StateIdle)
		stop();
}

bool Game::start(void)
{
	if (!m_physicsWorld.init(m_scene))
		return false;

	uv_timer_start(&m_frameTimer, &Game::frameCallback, 0, 16);
	uv_timer_start(&m_stateTimer, &Game::stateCallback, 1000, 1000);

	m_last = uv_now(m_loop);
	m_fps = 0;
	m_fpsCount = 0;
	m_elapsed = 0.0f;
	m_elapsedMin = 16.667f;

	m_state = StateLoading;

	return true;
}

void Game::stop(void)
{
	SGE_ASSERT(m_state != StateIdle);

	uv_timer_stop(&m_frameTimer);
	uv_timer_stop(&m_stateTimer);

	m_state = StateIdle;
}

bool Game::handleEvent(const SDL_Event &event)
{
	// TODO
	return false;
}

void Game::setName(const std::string &name)
{
	if (m_name == name)
		return;

	m_name = name;
	nameChanged(name);
}

void Game::setGravity(const glm::vec3 &v)
{
}

void Game::setFpsMax(int v)
{
	m_elapsedMin = 1000.0f / float(v);
	uv_timer_stop(&m_frameTimer);
	uv_timer_start(&m_frameTimer, &Game::frameCallback, 0, uint64_t(m_elapsedMin));
}

void Game::frame(float elapsed)
{
	m_fpsCount += 1;

	// m_physicsWorld.update(elapsed);
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

void Game::stateCallback(uv_timer_t *p)
{
	Game *game = ((Game *)(p->data));
	game->m_fps = game->m_fpsCount;
	game->m_fpsCount = 0;
}

SGE_END
