//
//
#include <stdio.h>
#include <SDL.h>
#include <sge.hpp>

static bool run = true;
static sge::gl::window window;
static sge::gl::context context;
static sge::scene::simple scene;
static sge::scene::camera camera;

static void handle_event(const SDL_Event &event)
{
	if (event.type == SDL_QUIT)
		run = false;

	window.handle_event(event);
}

int main(int argc, char *argv[])
{
	SDL_Event event;

	SDL_Init(SDL_INIT_EVERYTHING);

	window.init();
	context.init(window);
	scene.init();

	context.make_current();
	camera.init();
	camera.enable_background();
	camera.set_background(0.3f, 0.3f, 0.3f);
	context.swap_buffers();

	while (run) {
		while (SDL_PollEvent(&event))
			handle_event(event);

		scene.update(16.0f);
		scene.draw(&camera);

		context.make_current();
		camera.update();
		context.swap_buffers();

		SDL_Delay(16);
	}

	scene.shutdown();
	camera.shutdown();
	context.shutdown();
	window.shutdown();

	SDL_Quit();

	return 0;
}

