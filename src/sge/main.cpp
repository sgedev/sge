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
#include <sge/subsystem.hpp>
#include <sge/player.hpp>
#include <sge/editor.hpp>
#include <sge/server.hpp>

namespace player_subsystem {
	static void poll_event_cb(uv_prepare_t *p)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				uv_stop(uv_default_loop());
				continue;
			}
			sge::subsystem *subsys = (sge::subsystem *)p->data;
			SGE_ASSERT(subsys != NULL);
			subsys->feed_event(event);
		}
	}

	static void show_help(void)
	{
		SGE_LOGI("Playing game that inside [game path].\n");
		SGE_LOGI("Usage:\n");
		SGE_LOGI("  sge play [options] [game path]\n");
		SGE_LOGI("Options:\n");
		SGE_LOGI("  -h, --help\n");
		SGE_LOGI("    Show this message.\n");
	}

	static int main(const argh::parser &cmdline)
	{
		int ret;

		if (cmdline[{ "-h", "--help" }]) {
			show_help();
			return EXIT_SUCCESS;
		}

		if (cmdline.size() > 1) {
			show_help();
			return EXIT_FAILURE;
		}

		filesystem::path game_path(filesystem::path::getcwd());
		if (cmdline.size() == 1)
			game_path.set(cmdline[1]);

		if (!game_path.exists()) {
			SGE_LOGE("Invalid game path '%s'.\n", cmdline[1].c_str());
			return EXIT_FAILURE;
		}

		ret = SDL_Init(SDL_INIT_EVERYTHING);
		if (ret < 0) {
			SGE_LOGE("Failed to initialize SDL.\n");
			return EXIT_FAILURE;
		}

		auto subsys = std::make_unique<sge::player>(uv_default_loop());
		if (!subsys || !subsys->start(game_path)) {
			SGE_LOGE("Failed to initialize game player.\n");
			return EXIT_FAILURE;
		}

		uv_prepare_t poll_event_prepare;
		uv_prepare_init(uv_default_loop(), &poll_event_prepare);
		uv_prepare_start(&poll_event_prepare, poll_event_cb);
		poll_event_prepare.data = subsys.get();

		uv_run(uv_default_loop(), UV_RUN_DEFAULT);

		uv_prepare_stop(&poll_event_prepare);
		subsys->stop();

		SDL_Quit();

		return EXIT_SUCCESS;
	}
}

namespace editor_subsystem {
	static int main(const argh::parser &cmdline)
	{
		return EXIT_SUCCESS;
	}
}

namespace server_subsystem {
	static int main(const argh::parser &cmdline)
	{
		return EXIT_SUCCESS;
	}
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

static void show_help(void)
{
	SGE_LOGI("Usage:\n");
	SGE_LOGI("  sge [subcommand] [subcommand options]\n");
	SGE_LOGI("Subcommands:\n");
	SGE_LOGI("  play\n");
	SGE_LOGI("    Play a game.\n");
	SGE_LOGI("  edit\n");
	SGE_LOGI("    Edit a game.\n");
	SGE_LOGI("  server\n");
	SGE_LOGI("    Start a game server.\n");
	SGE_LOGI("  help\n");
	SGE_LOGI("    Show this message.\n");
	SGE_LOGI("  version\n");
	SGE_LOGI("    Show version message.\n");
}

int main(int argc, char *argv[])
{
	SDL_SetMainReady();

#ifdef SGE_DEBUG
	//SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	if (argc < 2) {
		show_help();
		return EXIT_FAILURE;
	}

	argh::parser cmdline(argc - 2, argv + 2);
	
	if (strcmp(argv[1], "play") == 0) {
		SGE_LOGD("Starting player...\n");
		return player_subsystem::main(cmdline);
	}

	if (strcmp(argv[1], "edit") == 0) {
		SGE_LOGD("Starting editor...\n");
		return editor_subsystem::main(cmdline);
	}

	if (strcmp(argv[1], "server") == 0) {
		SGE_LOGD("Starting server...\n");
		return server_subsystem::main(cmdline);
	}

	if (strcmp(argv[1], "help") == 0) {
		show_help();
		return EXIT_SUCCESS;
	}

	if (strcmp(argv[1], "version") == 0) {
		show_version();
		return EXIT_SUCCESS;
	}

	SGE_LOGE("Unknown subcommand '%s'.\n", argv[1]);

	return EXIT_FAILURE;
}

