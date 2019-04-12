//
//
#include <sge/gl.hpp>
#include <sge/scene/camera.hpp>

SGE_SCENE_CAMERA_BEGIN

static glm::mat4 project_matrix;

bool init(void)
{
	return true;
}

void shutdown(void)
{
}

void setup(void)
{
}

SGE_SCENE_CAMERA_END
