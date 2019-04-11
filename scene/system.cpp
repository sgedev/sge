//
//
#include <sge/scene/system.hpp>

SGE_SCENE_BEGIN

system::system(void)
{
}

system::~system(void)
{
}

bool system::init(void)
{
	m_renderer.init();

	m_camera.init(&m_renderer);

	return true;
}

void system::shutdown(void)
{
	m_renderer.shutdown();
}

void system::update(float elapsed)
{
}

void system::draw(void)
{
	m_renderer.begin();

	m_camera.setup();

	m_renderer.end();
}

SGE_SCENE_END

