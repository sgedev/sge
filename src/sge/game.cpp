//
//
#include <imgui.h>

#include <sge/game.hpp>

SGE_BEGIN

game::game(uv_loop_t *loop)
	: m_loop(loop)
	, m_window(NULL)
{
	SGE_ASSERT(m_loop != NULL);
}

game::~game(void)
{
}

bool game::init(window *pw)
{
	SGE_ASSERT(m_loop != NULL);
	SGE_ASSERT(pw != NULL);

	if (!m_renderer.init(pw))
		return false;

	m_input.init();
	m_scene.init();

	m_camera.perspective(90, 4.0f/3.0f, 0.01f, 100.0f);

	return true;
}

void game::shutdown(void)
{
	SGE_ASSERT(m_loop != NULL);

	m_scene.shutdown();
	m_input.shutdown();
}

void game::handle_event(const SDL_Event &event)
{
	m_input.handle_event(event);
}

void game::update(float elapsed)
{
	m_input.update(elapsed);
	m_scene.update(elapsed);
}

void game::draw(void)
{
	m_camera.clear();

	m_scene.draw(m_camera);

	m_camera.render();

	m_renderer.draw(m_camera);
}

void game::reset(void)
{
}

void game::clear(void)
{
}

bool game::load(const char *path)
{
	SGE_ASSERT(path != NULL);

	return false;
}

SGE_END

