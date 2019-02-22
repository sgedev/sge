//
//
#include <stdio.h>

#include <sge/gl.hpp>
#include <sge/gui.hpp>
#include <sge/renderer.hpp>
#include <sge/physics.hpp>
#include <sge/sound.hpp>
#include <sge/scene.hpp>
#include <sge/game.hpp>

#ifdef SGE_PROFILE
#	include "microprofile/microprofile.h"
#endif

SGE_BEGIN

static bool run;
static bool is_window_visibled;

static void handle_window_event(const SDL_WindowEvent *event)
{
	SGE_ASSERT(event != NULL);

	if (event->windowID != gl::get_window_id())
		return;

	switch (event->event) {
	case SDL_WINDOWEVENT_EXPOSED:
	case SDL_WINDOWEVENT_SHOWN:
		is_window_visibled = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
	case SDL_WINDOWEVENT_MINIMIZED:
		is_window_visibled = false;
		break;
	}
}

static void handle_event(const SDL_Event *event)
{
	SGE_ASSERT(event != NULL);

	switch (event->type) {
	case SDL_WINDOWEVENT:
		handle_window_event(&event->window);
		break;
	case SDL_QUIT:
		run = false;
		break;
	}

	gui::handle_event(event);
	renderer::handle_event(event);
	physics::handle_event(event);
	sound::handle_event(event);
	scene::handle_event(event);
	game::handle_event(event);
}

static void frame(float elapsed)
{
#ifdef SGE_PROFILE
	MICROPROFILE_SCOPEI("Frame", "Frame", MP_WHEAT);
#endif

	game::update(elapsed);
	physics::update(elapsed);
	sound::update(elapsed);
	scene::update(elapsed);

	if (is_window_visibled) {
		gl::draw_begin();
		game::draw();
		scene::draw();
		gui::draw();
		renderer::draw();
		gl::draw_end();
	}

#ifdef SGE_PROFILE
	MicroProfileFlip(0);
#endif
}

static int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return -1;

	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

	SGE_LOGI("SGE v%d.%d.%d initializing...\n",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);

	gl::init();
	renderer::init();
	gui::init();
	physics::init();
	sound::init();
	scene::init();
	game::init();

#ifdef SGE_PROFILE
	MicroProfileOnThreadCreate("Main");
	MicroProfileSetEnableAllGroups(true);
	MicroProfileSetForceMetaCounters(true);
	MicroProfileGpuInitGL();
#endif

	run = true;

	Uint32 pass;
	Uint32 base = SDL_GetTicks();
	SDL_Event event;

	while (run) {
		while (SDL_PollEvent(&event))
			handle_event(&event);
		pass = SDL_GetTicks() - base;
		if (pass > 16) {
			frame(float(pass) / 1000.0f);
			base = SDL_GetTicks();
		} else
			SDL_Delay(1);
	}

	game::shutdown();
	scene::shutdown();
	sound::shutdown();
	physics::shutdown();
	gui::shutdown();
	renderer::shutdown();
	gl::shutdown();

	SGE_LOGI("SGE shutdown.\n");

	SDL_Quit();

	return 0;
}

SGE_END

int main(int argc, char *argv[])
{
	return sge::main(argc, argv);
}
