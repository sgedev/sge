//
//
#define WIN32_LEAN_AND_MEAN
#define SDL_MAIN_HANDLED

#include <cstdio>
#include <memory>
#include <string>
#include <iostream>

#include <argh.h>
#include <filesystem/path.h>
#include <filesystem/resolver.h>

#include <SDL.h>
#include <physfs.h>

#include <sge/common.hpp>
#include <sge/window.hpp>
#include <sge/game.hpp>
#include <sge/editor.hpp>

SGE_BEGIN

typedef void (*handle_event_func)(const SDL_Event *event);
typedef void (*update_func)(float elapsed);
typedef void (*draw_func)(void);
typedef void (*shutdown_func)(void);

static uv_timer_t frame_timer;
static uv_timer_t state_timer;
static bool editor_enabled;
static uint64_t last;
static int fps;
static int fps_count;

static handle_event_func handle_event;
static update_func update;
static draw_func draw;
static shutdown_func shutdown;

static void poll_events(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			SGE_LOGI("quit...\n");
			uv_stop(uv_default_loop());
			continue;
		}

		window::handle_event(&event);
		handle_event(&event);
	}
}

static void frame(uv_timer_t *p)
{
	poll_events();

	uint64_t elapsed = uv_now(uv_default_loop()) - last;

	update(float(elapsed) / 1000.0f);

	if (window::draw_begin()) {
		draw();
		window::draw_end();
	}

	fps_count += 1;
	uv_update_time(uv_default_loop());
	last = uv_now(uv_default_loop());
}

static void state(uv_timer_t* p)
{
	fps = fps_count;
	fps_count = 0;
}

static bool start(void)
{
	if (!window::init())
		return false;

	if (editor_enabled) {
		editor::init();
		handle_event = editor::handle_event;
		update = editor::update;
		draw = editor::draw;
		shutdown = editor::shutdown;
	} else {
		game::init();
		handle_event = game::handle_event;
		update = game::update;
		draw = game::draw;
		shutdown = game::shutdown;
	}

	uv_timer_init(uv_default_loop(), &frame_timer);
	uv_timer_init(uv_default_loop(), &state_timer);

	uv_timer_start(&frame_timer, frame, 0, 10);
	uv_timer_start(&state_timer, state, 1000, 1000);

	uv_update_time(uv_default_loop());
	last = uv_now(uv_default_loop());

	return true;
}

static void stop(void)
{
	uv_timer_stop(&state_timer);
	uv_timer_stop(&frame_timer);

	shutdown();

	window::shutdown();
}

SGE_END

static void show_version(void)
{
#ifdef SGE_DEBUG
	const char *release = "debug";
#else
	const char *release = "release"
#endif

	SGE_LOGI("simple game engine v%d.%d.%d [%s]\n",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH, release);
}

static void show_help(void)
{
	SGE_LOGI("usage: sge [options] [game path]\n");
	show_version();

	SGE_LOGI("\noptions:\n");
	SGE_LOGI("  -e, --edit\n");
	SGE_LOGI("    edit the game.\n");
	SGE_LOGI("  --debug\n");
	SGE_LOGI("    enable debug logs.\n");
	SGE_LOGI("  --quiet\n");
	SGE_LOGI("    disable all logs.\n");
	SGE_LOGI("  -h, --help\n");
	SGE_LOGI("    show this message.\n");
	SGE_LOGI("  -v, --version\n");
	SGE_LOGI("    show version.\n");
}

static bool logger_enabled = true;

static void logger(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
	if (!logger_enabled)
		return;

	if (category == SDL_LOG_CATEGORY_ERROR || priority == SDL_LOG_PRIORITY_ERROR)
		fprintf(stderr, "%s\n", message);
	else
		fprintf(stdout, "%s\n", message);
}

int main(int argc, char *argv[])
{
	int ret;
	argh::parser cmdline(argv);

	SDL_SetMainReady();
	SDL_LogSetOutputFunction(logger, NULL);

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#else
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
#endif

	if (cmdline[{ "-h", "--help" }]) {
		show_help();
		return EXIT_SUCCESS;
	}

	if (cmdline[{ "-v", "--version" }]) {
		show_version();
		return EXIT_SUCCESS;
	}

	if (cmdline[{ "--debug" }])
		SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

	if (cmdline[{ "--quiet" }])
		logger_enabled = false;

	filesystem::path game_path;
	switch (cmdline.size()) {
	case 2:
		game_path.set(cmdline[1]);
		break;
	case 1:
		game_path = filesystem::path::getcwd();
		break;
	default:
		SGE_LOGE("too many game path.\n");
		return EXIT_FAILURE;
	}

	game_path = game_path.make_absolute();
	if (!game_path.exists()) {
		SGE_LOGE("invalid game path '%s'.\n", cmdline[1].c_str());
		return EXIT_FAILURE;
	}

	SGE_LOGI("game path: '%s'.\n", game_path.str().c_str());

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0) {
		SGE_LOGE("failed to initialize sdl.\n");
		return EXIT_FAILURE;
	}

	ret = PHYSFS_init(NULL);
	if (!ret) {
		SGE_LOGE("Failed to initialize PhysFS.\n");
		SDL_Quit();
		return EXIT_FAILURE;
	}

	ret = PHYSFS_mount(game_path.str().c_str(), NULL, 0);
	if (!ret) {
		SGE_LOGE("Failed to mount game path.\n");
		PHYSFS_deinit();
		SDL_Quit();
		return EXIT_FAILURE;
	}

	if (cmdline[{ "-e", "--edit" }])
		sge::editor_enabled = true;

	if (!sge::start()) {
		SGE_LOGE("initialize failed.\n");
		PHYSFS_deinit();
		SDL_Quit();
		return EXIT_FAILURE;
	}

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	sge::stop();

	PHYSFS_deinit();
	SDL_Quit();

	return EXIT_SUCCESS;
}
