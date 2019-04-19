//
//
#include <sge/scene/camera.hpp>

SGE_SCENE_BEGIN

camera::camera(void)
	: m_background(0.0f, 0.0f, 0.0f)
	, m_background_enabled(false)
{
}

camera::~camera(void)
{
}

bool camera::init(void)
{
	m_view.init();

	return true;
}

void camera::shutdown(void)
{
	m_view.shutdown();
}

void camera::update(void)
{
	if (m_background_enabled) {
		m_view.enable_clear();
		m_view.set_clear_color(m_background);
	}

	m_view.set_transform(get_transform());

	m_view.update();
}

SGE_SCENE_END

