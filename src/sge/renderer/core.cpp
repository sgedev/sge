//
//
#include <sge/renderer.hpp>

SGE_RENDERER_BEGIN

fb default_target;
light ambient_light;

bool init(void)
{
	if (!vg::init())
		return false;

	return true;
}

void shutdown(void)
{
	vg::shutdown();
}

void begin(void)
{
    // TODO reset 3d.

    vg::reset();
}

void end(void)
{
    glViewport(0, 0, default_target.width(), default_target.height());
    glClear(GL_COLOR_BUFFER_BIT);

    // TODO render 3d.

    vg::render();
}

void add(light *p)
{
    SGE_ASSERT(p != nullptr);
}

void add(mesh *p)
{
    SGE_ASSERT(p != nullptr);
}

SGE_RENDERER_END
