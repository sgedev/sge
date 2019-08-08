//
//
#define WIN32_LEAN_AND_MEAN
#define SDL_MAIN_HANDLED

#include <memory>
#include <string>
#include <iostream>

#include <argh.h>
#include <filesystem/path.h>
#include <filesystem/resolver.h>

#include <SDL.h>

#include <sge/common.hpp>
#include <sge/game.hpp>
#include <sge/editor.hpp>

static void poll_event_cb(uv_prepare_t *p)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			uv_stop(uv_default_loop());
			continue;
		}
		sge::game *game = (sge::game *)p->data;
		SGE_ASSERT(game != NULL);
		game->feed_event(event);
	}
}

static void show_help(void)
{
	SGE_LOGI("Usage:\n");
	SGE_LOGI("  sge [options] [game path]\n");
	SGE_LOGI("Options:\n");
	SGE_LOGI("  -e, --edit\n");
	SGE_LOGI("    Edit the game.\n");
	SGE_LOGI("  -h, --help\n");
	SGE_LOGI("    Show this message.\n");
}

static void show_version(void)
{
#ifdef SGE_DEBUG
	const char *release = "Debug";
#else
	const char *release = "Release"
#endif

	SGE_LOGI("Simple Game Engine v%d.%d.%d [%s]\n",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH, release);
}

int main(int argc, char *argv[])
{
	int ret;
	argh::parser cmdline(argv);

	SDL_SetMainReady();

//#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
//#endif

	if (cmdline[{ "-h", "--help" }]) {
		show_help();
		return EXIT_SUCCESS;
	}

	filesystem::path game_path(filesystem::path::getcwd());
	if (cmdline.size() == 1) {
		game_path.set(cmdline[1]);
		game_path = game_path.make_absolute();
	}

	SGE_LOGI("Game path: '%s'.\n", game_path.str().c_str());

	if (!game_path.exists()) {
		SGE_LOGE("Invalid game path '%s'.\n", cmdline[1].c_str());
		return EXIT_FAILURE;
	}

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0) {
		SGE_LOGE("Failed to initialize SDL.\n");
		return EXIT_FAILURE;
	}

	std::unique_ptr<sge::game> game;
	if (cmdline[{ "-e", "--edit" }])
		game = std::make_unique<sge::editor>(uv_default_loop(), game_path);
	else
		game = std::make_unique<sge::game>(uv_default_loop(), game_path);
	if (!game || !game->start()) {
		SGE_LOGE("Failed to initialize game player.\n");
		SDL_Quit();
		return EXIT_FAILURE;
	}

	uv_prepare_t poll_event_prepare;
	uv_prepare_init(uv_default_loop(), &poll_event_prepare);
	uv_prepare_start(&poll_event_prepare, poll_event_cb);
	poll_event_prepare.data = game.get();

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_prepare_stop(&poll_event_prepare);

	game->stop();

	SDL_Quit();

	return EXIT_SUCCESS;
}

