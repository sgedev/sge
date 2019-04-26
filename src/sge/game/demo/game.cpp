//
//
#include <sge/game.hpp>

SGE_GAME_BEGIN

static void move_left(SDL_Keycode key, void *data)
{
	fps::move_left();
}

static void move_right(SDL_Keycode key, void *data)
{
	fps::move_right();
}

static void move_forward(SDL_Keycode key, void *data)
{
	fps::move_forward();
}

static void move_backward(SDL_Keycode key, void *data)
{
	fps::move_backward();
}

static void mouse_look(float xoffset, float yoffset, void *data)
{
	fps::mouse_look(xoffset, yoffset);
}

bool init(void)
{
	fps::init();
	fps::set_move_speed(8.0f);
	fps::set_mouse_sensitivity(0.001f);

	input::init();
	input::set_mouse_motion_handler(mouse_look, NULL);
	input::bind(SDLK_a, move_left, NULL);
	input::bind(SDLK_s, move_backward, NULL);
	input::bind(SDLK_d, move_right, NULL);
	input::bind(SDLK_w, move_forward, NULL);

	return true;
}

void shutdown(void)
{
	input::shutdown();
	fps::shutdown();
}

void update(void)
{
	input::update();
	fps::update();
}

void handle_event(const SDL_Event &event)
{
	input::handle_event(event);
}

SGE_GAME_END

