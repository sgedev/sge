//
//
#include <sge/renderer.hpp>
#include <sge/scene.hpp>
#include <sge/game.hpp>
#include <sge.hpp>

SGE_BEGIN

static int trapFps(void)
{
	return 123;
}

bool init(void)
{
	logInfo("Initializing...\n");

	if (!Renderer::init())
		goto bad0;

	if (!Scene::init())
		goto bad1;

	if (!Game::init())
		goto bad2;

	return true;

bad2:
	Scene::shutdown();

bad1:
	Renderer::shutdown();

bad0:
	return false;
}

void shutdown(void)
{
	Game::shutdown();
	Scene::shutdown();
	Renderer::shutdown();

	logInfo("Shutdown.\n");
}

bool handleEvent(const SDL_Event &event)
{
	bool ret = false;

	ret = ret || Renderer::handleEvent(event);
	ret = ret || Game::handleEvent(event);

	return ret;
}

void frame(float elapsed)
{
	static const Game::TrapHandler trapHandler = {
		trapFps
	};

	Game::update(elapsed, &trapHandler);

	Scene::update(elapsed);

	if (Renderer::beginFrame()) {
		Scene::draw();
		Renderer::endFrame();
	}
}

SGE_END
