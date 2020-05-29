//
//
#include <stdio.h>
#include <stdlib.h>

#include <memory>
#include <string>

#include <argh.h>
#include <physfs.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <SGE/Game.hpp>
#include <SGE/Client.hpp>
#include <SGE/Server.hpp>

struct PHYSFS_Initialization {
	PHYSFS_Initialization(const char *argv0)
	{
		PHYSFS_init(argv0);
	}

	~PHYSFS_Initialization(void)
	{
		if (PHYSFS_isInit())
			PHYSFS_deinit();
	}

	operator bool(void) const
	{
		return PHYSFS_isInit();
	}
};

struct SDL_Initialization {
	SDL_Initialization(Uint32 flags)
	{
		SDL_Init(flags);
	}

	~SDL_Initialization(void)
	{
		if (SDL_WasInit(0))
			SDL_Quit();
	}

	operator bool(void) const
	{
		return (SDL_WasInit(0));
	}
};

static void sgePollEvents(uv_prepare_t *p)
{
	SDL_Event event;

	SGE::Game *game = (SGE::Game *)(p->data);
	SGE_ASSERT(game != NULL);

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			uv_stop(p->loop);
			break;
		default:
			game->handleEvent(event);
			break;
		}
	}
}

static void sgePrintHelp(void)
{
	SGE_LOGI("Usage:");
	SGE_LOGI("  sge [options] <game path>");
	SGE_LOGI("Options:");
	SGE_LOGI("  -s, --server");
	SGE_LOGI("    start as a server.");
	SGE_LOGI("  -d, --debug");
	SGE_LOGI("    enable debug log level.");
	SGE_LOGI("  -h, --help");
	SGE_LOGI("    Show this message.");
	SGE_LOGI("  -v, --version");
	SGE_LOGI("    Show version.");
}

static void sgePrintVersion(void)
{
	SGE_LOGI("%d.%d.%d",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);
}

static void sgePrintBanner(void)
{
	SGE_LOGI("Simple Game Engine V%d.%d.%d",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);
}

static void sgePrintLog(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
	if (priority == SDL_LOG_PRIORITY_ERROR || priority == SDL_LOG_PRIORITY_CRITICAL)
		fprintf(stderr, "SGE: %s\n", message);
	else
		fprintf(stdout, "SGE: %s\n", message);
}

int main(int argc, char *argv[])
{
	SDL_LogSetOutputFunction(sgePrintLog, NULL);

	argh::parser cmdline;

	if (cmdline[{ "-d", "--debug"}])
		SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	if (cmdline[{ "-h", "--help"}]) {
		sgePrintHelp();
		return EXIT_SUCCESS;
	}

	if (cmdline[{ "-v", "--version"}]) {
		sgePrintVersion();
		return EXIT_SUCCESS;
	}

	if (cmdline.size() < 2) {
		SGE_LOGE("Game path is not specified.");
		return EXIT_FAILURE;
	}

	sgePrintBanner();

	PHYSFS_Initialization physfs(argv[0]);
	if (!physfs) {
		SGE_LOGE("Failed to init PhysFS.");
		return EXIT_FAILURE;
	}

	SDL_Initialization sdl(SDL_INIT_EVERYTHING);
	if (!sdl) {
		SGE_LOGE("Failed to init SDL.");
		return EXIT_FAILURE;
	}

	std::string mode;
	std::unique_ptr<SGE::Game> game;

	cmdline({ "-m", "--mode" }, "play") >> mode;
	SGE_LOGI("Mode: %s", mode.c_str());
	SGE_LOGI("Path: %s", cmdline[1].c_str());

	if (cmdline[{ "-s", "--server"}])
		game.reset(new SGE::Server(uv_default_loop()));
	else
		game.reset(new SGE::Client(uv_default_loop()));

	if (!game) {
		SGE_LOGE("Failed to init SGE.");
		return EXIT_FAILURE;
	}

	if (!game->start(cmdline[1])) {
		SGE_LOGE("Failed to start game.");
		return EXIT_FAILURE;
	}

	uv_prepare_t eventPoller;
	uv_prepare_init(uv_default_loop(), &eventPoller);
	uv_prepare_start(&eventPoller, sgePollEvents);
	eventPoller.data = game.get();

	// Main loop.
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_prepare_stop(&eventPoller);
	
	return EXIT_SUCCESS;
}
