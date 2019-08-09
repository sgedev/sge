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
#include <sge/player.hpp>
#include <sge/editor.hpp>

static void poll_event_cb(uv_prepare_t *p)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			SGE_LOGI("quit...\n");
			uv_stop(uv_default_loop());
			continue;
		}
		sge::player *player = (sge::player *)p->data;
		SGE_ASSERT(player != NULL);
		player->feed_event(event);
	}
}

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

	PHYSFS_init(NULL);

	std::unique_ptr<sge::player> p;

	if (cmdline[{ "-e", "--edit" }])
		p = std::make_unique<sge::editor>(uv_default_loop());
	else
		p = std::make_unique<sge::player>(uv_default_loop());

	if (!p || !p->start()) {
		SGE_LOGE("initialize failed.\n");
		SDL_Quit();
		return EXIT_FAILURE;
	}

	uv_prepare_t poll_event_prepare;
	uv_prepare_init(uv_default_loop(), &poll_event_prepare);
	uv_prepare_start(&poll_event_prepare, poll_event_cb);
	poll_event_prepare.data = p.get();

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_prepare_stop(&poll_event_prepare);
	p->stop();

	SDL_Quit();

	return EXIT_SUCCESS;
}

