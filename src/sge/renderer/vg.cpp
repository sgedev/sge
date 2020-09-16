//
//
#include <sge/renderer/vg.hpp>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg.h>
#include <nanovg_gl.h>

SGE_RENDERER_VG_BEGIN

static struct NVGcontext *vg;

bool init(void)
{
	SGE_ASSERT(vg == nullptr);

	vg = nvgCreateGL3(0);

	return true;
}

void shutdown(void)
{
	SGE_ASSERT(vg != nullptr);

	nvgDeleteGL3(vg);
	vg = nullptr;
}

void reset(void)
{
	SGE_ASSERT(vg != nullptr);
}

void render(void)
{
	SGE_ASSERT(vg != nullptr);
}

SGE_RENDERER_VG_END
