//
//
#include <memory>
#include <string>
#include <iostream>

#include <argh.h>
#include <physfs.h>
#include <filesystem/path.h>
#include <filesystem/resolver.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <sge/common.hpp>
#include <sge/subsystem.hpp>
#include <sge/player.hpp>
#include <sge/editor.hpp>
#include <sge/server.hpp>

SGE_BEGIN

typedef std::shared_ptr<subsystem> subsystem_ptr;

static subsystem_ptr make_player(uv_loop_t *loop, const argh::parser &cmdline)
{
	auto p = std::make_shared<player>(loop);
	if (p) {
		// process player cmdline.
	}

	return p;
}

static subsystem_ptr make_editor(uv_loop_t *loop, const argh::parser &cmdline)
{
	auto p = std::make_shared<editor>(loop);
	if (p) {
		// process editor cmdline.
	}

	return p;
}

static subsystem_ptr make_server(uv_loop_t *loop, const argh::parser &cmdline)
{
	auto p = std::make_shared<server>(loop);
	if (p) {
		// process server cmdline.
	}

	return p;
}

static void poll_event_cb(uv_prepare_t *p)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			uv_stop(uv_default_loop());
			continue;
		}
		subsystem *subsys = (subsystem *)p->data;
		SGE_ASSERT(subsys != NULL);
		subsys->feed_event(event);
	}
}

static void show_version(void)
{
	SGE_LOGI("simple game engine v%d.%d.%d ["
#ifdef SGE_DEBUG
		"debug version"
#else
		"release version"
#endif
		"]\n",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);

	SGE_LOGD("platform: %s, cpu %d, memory %dMB\n",
		SDL_GetPlatform(), SDL_GetCPUCount(), SDL_GetSystemRAM());
}

static void show_help(void)
{
	SGE_LOGI("usage:\n");
	SGE_LOGI("  sge [subcommand] [subcommand options]\n");
	SGE_LOGI("subcommands:\n");
	SGE_LOGI("  play\n");
	SGE_LOGI("    play a game.\n");
	SGE_LOGI("  edit\n");
	SGE_LOGI("    edit a game.\n");
	SGE_LOGI("  server\n");
	SGE_LOGI("    start a game server.\n");
	SGE_LOGI("common options:\n");
	SGE_LOGI("  -h, --help\n");
	SGE_LOGI("    show this message or subcommand help messages.\n");
	SGE_LOGI("  -v, --version\n");
	SGE_LOGI("    show version.\n");
	SGE_LOGI("  -d, --debug\n");
	SGE_LOGI("    show debug logs.\n");
}

static int main(const argh::parser &cmdline)
{
#ifndef SGE_DEBUG
	if (cmdline[{ "-d", "--debug" }])
#endif
		SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

	if (cmdline[{ "-v", "--version" }]) {
		show_version();
		return EXIT_SUCCESS;
	}

	show_version();

	if (cmdline[{ "-h", "--help" }]) {
		show_help();
		return EXIT_SUCCESS;
	}

	if (cmdline.size() < 2) {
		show_help();
		return EXIT_FAILURE;
	}

	subsystem_ptr subsys;

	if (cmdline[1] == "play")
		subsys = make_player(uv_default_loop(), cmdline);
	else if (cmdline[1] == "edit")
		subsys = make_editor(uv_default_loop(), cmdline);
	else if (cmdline[1] == "server")
		subsys = make_server(uv_default_loop(), cmdline);
	else {
		SGE_LOGE("unknown subcommand '%s'.\n", cmdline[1].c_str());
		return EXIT_FAILURE;
	}

	if (!subsys || !subsys->start()) {
		SGE_LOGE("failed to initialize subsystem.\n");
		return EXIT_FAILURE;
	}

	uv_prepare_t poll_event_prepare;
	uv_prepare_init(uv_default_loop(), &poll_event_prepare);
	uv_prepare_start(&poll_event_prepare, poll_event_cb);
	poll_event_prepare.data = subsys.get();

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_prepare_stop(&poll_event_prepare);
	subsys->stop();

	return EXIT_SUCCESS;
}

SGE_END

int main(int argc, char *argv[])
{
	int ret;
	argh::parser cmdline(argc, argv);

	SDL_SetMainReady();

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0) {
		fprintf(stderr, "failed to initialize sdl.\n");
		return EXIT_FAILURE;
	}

	PHYSFS_init(NULL);

	ret = sge::main(cmdline);

	PHYSFS_deinit();

	SDL_Quit();

	return ret;
}

