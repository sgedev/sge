//
//
#include <sge/input.hpp>
#include <sge/scene.hpp>
#include <sge/game.hpp>

SGE_GAME_BEGIN

static void move_left(SDL_Keycode key, void *data)
{
	scene::camera::move_left();
}

static void move_right(SDL_Keycode key, void *data)
{
	scene::camera::move_right();
}

static void move_forward(SDL_Keycode key, void *data)
{
	scene::camera::move_forward();
}

static void move_backward(SDL_Keycode key, void *data)
{
	scene::camera::move_backward();
}

static void mouse_look(float xoffset, float yoffset, void *data)
{
	scene::camera::mouse_look(xoffset, yoffset);
}

bool init(void)
{
	scene::camera::set_move_speed(8.0f);

	input::set_mouse_sensitivity(0.1f);
	input::set_mouse_motion_handler(mouse_look, NULL);

	input::bind(SDLK_a, move_left, NULL);
	input::bind(SDLK_s, move_backward, NULL);
	input::bind(SDLK_d, move_right, NULL);
	input::bind(SDLK_w, move_forward, NULL);

	return true;
}

void shutdown(void)
{
}

void update(void)
{
}

bool can_quit(void)
{
	return true;
}

SGE_GAME_END
