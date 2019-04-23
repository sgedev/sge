//
//
#include <sge/scene/camera.hpp>

SGE_SCENE_BEGIN

camera::camera(void)
{
}

camera::~camera(void)
{
}

bool camera::init(void)
{
	m_view.init();

	static int i = 0;

	i++;

	if (i == 1)
		m_view.set_viewport(0, 0, 100, 100);
	if (i == 2)
		m_view.set_viewport(200, 200, 100, 100);

	return true;
}

void camera::shutdown(void)
{
	m_view.shutdown();
}

void camera::update(void)
{
	m_view.set_transform(get_transform());

	m_view.update();
}

SGE_SCENE_END

