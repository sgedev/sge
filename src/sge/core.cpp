//
//
#include <sge.hpp>

SGE_BEGIN

bool init(void)
{
	if (!Renderer::init())
		goto bad0;

	return true;

bad0:
	return false;
}

void shutdown(void)
{
	Renderer::shutdown();
}

const char *lastError(void)
{
	return "no error";
}

bool handleEvent(const SDL_Event &evt)
{
	Renderer::handleEvent(evt);

	return true;
}

void update(float elapsed)
{
	if (Renderer::beginFrame()) {
		// TODO
		Renderer::endFrame();
	}
}

SGE_END

