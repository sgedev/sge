//
//
#include <stdio.h>

#include <sge/fs.h>
#include <sge/gl.h>
#include <sge/gui.h>
#include <sge/renderer.h>
#include <sge/physics.h>
#include <sge/sound.h>
#include <sge/game.h>

SGE_BEGIN

static bool run;
static Uint32 curr_time;
static unsigned int frame_per_second;
static unsigned int frame_elapsed_min;
static float frame_elapsed;
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
	game::handle_event(event);
}

static void update_window(void)
{
	if (!is_window_visibled)
		return;

	gl::draw_begin();

	game::draw();
	gui::draw();
	renderer::draw();

	gl::draw_end();
}

static void main_loop(void)
{
	run = true;
	curr_time = SDL_GetTicks();
	frame_elapsed = 0.0f;
	frame_per_second = 0;

	SDL_Event event;
	Uint32 elapsed;
	Uint32 last = curr_time;
	Uint32 fps_last = curr_time;
	unsigned long fps_count = 0;

	SGE_LOGD("Main loop started.\n");

	while (run) {
		while (SDL_PollEvent(&event))
			handle_event(&event);

		curr_time = SDL_GetTicks();
		elapsed = curr_time - last;
		if (elapsed < frame_elapsed_min) {
			SDL_Delay(1);
			continue;
		}

		frame_elapsed = float(elapsed) / 1000.0f;

		physics::update();
		renderer::update();
		sound::update();
		game::update();

		update_window();

		last = SDL_GetTicks();
		fps_count++;

		if (fps_last < last && (last - fps_last) >= 250) {
			frame_per_second = fps_count << 2;
			fps_last = last;
			fps_count = 0;
		}
	}

	SGE_LOGD("Main loop exited.\n");
}

static void show_info(void)
{
	SGE_LOGI("SGE %d.%d.%d Copyright(C) 2019\n",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);

	// TODO print licenses.
	SGE_LOGI("<<LICENSES>>\n");

#ifdef SGE_DEBUG
	SGE_LOGI("Debug enabled.\n");
#endif

	SGE_LOGI("Game: \"%s\"\n", SGE_GAME_NAME);
}

static int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return -1;

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	show_info();

	set_fps_max(60);

	game::pre_init();

	fs::init("new.db");
	gl::init();
	renderer::init();
	gui::init();
	physics::init();
	sound::init();
	game::init();

	main_loop();

	game::shutdown();
	sound::shutdown();
	physics::shutdown();
	gui::shutdown();
	renderer::shutdown();
	gl::shutdown();
	fs::shutdown();

	game::post_shutdown();

	SDL_Quit();

	return 0;
}

void set_fps_max(unsigned int v)
{
	SGE_ASSERT(v > 0);
	SGE_LOGD("Set max fps to %d\n", v);

	frame_elapsed_min = 1005 / v;
}

Uint32 now(void)
{
	return curr_time;
}

float elapsed(void)
{
	return frame_elapsed;
}

unsigned int fps(void)
{
	return 	frame_per_second;
}

unsigned int fps_max(void)
{
	return 1000 / frame_elapsed_min;
}

SGE_END

int main(int argc, char *argv[])
{
	return sge::main(argc, argv);
}
