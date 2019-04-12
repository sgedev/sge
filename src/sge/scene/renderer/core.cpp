//
//
#include <sge/gl.hpp>
#include <sge/scene/renderer.hpp>

SGE_SCENE_RENDERER_BEGIN

static gl::program default_prop;

bool init(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	

	return true;
}

void shutdown(void)
{
}

void begin(void)
{
}

void end(void)
{
}

SGE_SCENE_RENDERER_END

