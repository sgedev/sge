//
//
#include <future>

#include <imgui.h>
#include <lua.hpp>

#include <sge/scene.hpp>
#include <sge/game.hpp>

#include "input.hpp"
#include "vm.hpp"

SGE_GAME_BEGIN

static vm s_init;

bool init(void)
{
	input::init();

	if (!s_init.start("/init.lua"))
		return false;

	return false;
}

void shutdown(void)
{
	s_init.stop();

	input::shutdown();
}

bool handle_event(const SDL_Event &event)
{
	if (event.type == SDL_QUIT) {
		uv_stop(uv_default_loop());
		return true;
	}

	return input::handle_event(event);
}

void update(float elapsed)
{
	input::update(elapsed);

	// TODO game logic update.

	scene::update(elapsed);
}

void draw(void)
{
	scene::draw();
}

SGE_GAME_END

