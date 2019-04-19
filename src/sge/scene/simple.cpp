//
//
#include <sge/scene/simple.hpp>

SGE_SCENE_BEGIN

simple::simple(void)
{
}

simple::~simple(void)
{
}

bool simple::init(void)
{
	return true;
}

void simple::shutdown(void)
{
}

void simple::update(float elapsed)
{
}

void simple::draw(camera *cam)
{
	SGE_ASSERT(cam != NULL);

	m_camera = cam;

	m_camera = NULL;
}

SGE_SCENE_END

