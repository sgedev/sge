//
//
#include <sge/scene.hpp>

SGE_BEGIN

scene::scene(void)
{
}

scene::~scene(void)
{
}

bool scene::init(void)
{
	return true;
}

void scene::shutdown(void)
{
}

void scene::update(float elapsed)
{
}

void scene::shot(camera &cam)
{
	// add scene to camera.
}

SGE_END

