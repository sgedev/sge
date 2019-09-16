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
#include <GL/glex.h>

#include <SDL.h>
#include <physfs.h>
#include <sge.hpp>

static void poll_events_cb(uv_prepare_t *p)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			SGE_LOGI("quit...\n");
			uv_stop(uv_default_loop());
			continue;
		}

		((sge::subsystem *)(p->data))->feed_event(&event);
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
	uv_prepare_t poll_events_prepare;

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

	ret = glexInit(NULL);
	if (ret < 0) {
		SGE_LOGE("Failed to initialize GLEX.\n");
		PHYSFS_deinit();
		SDL_Quit();
		return EXIT_FAILURE;
	}

	sge::subsystem *subsys;

	if (cmdline[{ "--edit" }])
		subsys = new sge::editor(uv_default_loop());
	else if (cmdline[{ "--server" }])
		subsys = new sge::server(uv_default_loop());
	else
		subsys = new sge::player(uv_default_loop());

	if (!subsys || !subsys->start()) {
		SGE_LOGE("initialize failed.\n");
		delete subsys;
		glexShutdown();
		PHYSFS_deinit();
		SDL_Quit();
		return EXIT_FAILURE;
	}

	uv_prepare_init(uv_default_loop(), &poll_events_prepare);
	poll_events_prepare.data = subsys;
	uv_prepare_start(&poll_events_prepare, poll_events_cb);

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_prepare_stop(&poll_events_prepare);

	subsys->stop();
	delete subsys;

	glexShutdown();
	PHYSFS_deinit();
	SDL_Quit();

	return EXIT_SUCCESS;
}
