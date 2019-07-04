//
//
#include <future>

#include <lua.hpp>

#include <sge/gui.hpp>
#include <sge/scene.hpp>
#include <sge/game.hpp>

#include "input.hpp"
#include "vm.hpp"

SGE_GAME_BEGIN

static vm s_init;

bool init(void)
{
	gui::init();
	scene::init();

	input::init();

	if (!s_init.start("/init.lua"))
		return false;

	return false;
}

void shutdown(void)
{
	s_init.stop();

	input::shutdown();

	scene::shutdown();
	gui::shutdown();
}

bool handle_event(const SDL_Event &event)
{
	if (event.type == SDL_QUIT) {
		uv_stop(uv_default_loop());
		return true;
	}

	gui::handle_event(event);
	input::handle_event(event);
}

void update(float elapsed)
{
	input::update(elapsed);

	// TODO game logic update.

	scene::update(elapsed);
	gui::update(elapsed);
}

void draw(void)
{
	scene::draw();
	gui::draw();
}

SGE_GAME_END

