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

bool camera::init(renderer *r)
{
	SGE_ASSERT(r != NULL);

	m_renderer = r;

	return true;
}

void camera::shutdown(void)
{
}

void camera::setup(void)
{
	SGE_ASSERT(m_renderer != NULL);
}

SGE_SCENE_END

