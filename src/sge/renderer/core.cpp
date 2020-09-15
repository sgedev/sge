//
//
#include <sge/renderer.hpp>

SGE_RENDERER_BEGIN

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
	vg::begin();
}

void end(void)
{
	
	vg::end();
}

SGE_RENDERER_END
