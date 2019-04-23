//
//
#include <stdio.h>
#include <SDL.h>
#include <sge.hpp>

static bool run = true;
static sge::gl::window window;
static sge::gl::context context;
static sge::scene::simple scene;
static sge::scene::camera camera1;
static sge::scene::camera camera2;

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
	camera1.init();
	camera1.enable_background();
	camera1.set_background(0.1f, 0.1f, 0.1f);
	camera2.init();
	camera2.enable_background();
	camera2.set_background(0.9f, 0.9f, 0.9f);
	context.swap_buffers();

	while (run) {
		while (SDL_PollEvent(&event))
			handle_event(event);

		scene.update(16.0f);

		scene.draw(&camera1);
		scene.draw(&camera2);

		context.make_current();
		camera1.update();
		camera2.update();
		context.swap_buffers();

		SDL_Delay(16);
	}

	camera2.shutdown();
	camera1.shutdown();
	scene.shutdown();
	context.shutdown();
	window.shutdown();

	SDL_Quit();

	return 0;
}

