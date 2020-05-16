//
//
#include <stdio.h>
#include <stdlib.h>

#include <memory>
#include <string>

#include <argh.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <SGE/Handle.hpp>
#include <SGE/Player.hpp>
#include <SGE/Editor.hpp>
#include <SGE/Server.hpp>

struct SDL_Initialization {
	int state;

	SDL_Initialization(Uint32 flags = SDL_INIT_EVERYTHING)
	{
		state = SDL_Init(flags);
	}

	~SDL_Initialization(void)
	{
		if (state == 0)
			SDL_Quit();
	}

	operator bool(void) const
	{
		return (state == 0);
	}
};

static void sgePollEvents(uv_prepare_t *p)
{
	SDL_Event event;

	SGE::Handle *handler = (SGE::Handle *)(p->data);
	SGE_ASSERT(handler != NULL);

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			uv_stop(p->loop);
			break;
		default:
			handler->handleEvent(event);
			break;
		}
	}
}

static void sgePrintHelp(void)
{
	SGE_LOGI("Usage:");
	SGE_LOGI("  sge [options] <game path>");
	SGE_LOGI("Options:");
	SGE_LOGI("  -m, --mode");
	SGE_LOGI("    play  - play a game, this is default mode.");
	SGE_LOGI("    edit  - edit a game.");
	SGE_LOGI("    serve - start a game server.");
	SGE_LOGI("  -d, --debug");
	SGE_LOGI("    debug mode for developer.");
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
	cmdline.add_params({ "-m", "--mode" });
	cmdline.parse(argc, argv);

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#else
	if (cmdline[{ "-d", "--debug"}])
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
		SGE_LOGE("Game path is not specified. %d", cmdline.size());
		return EXIT_FAILURE;
	}

	sgePrintBanner();

	SDL_Initialization sdl;
	if (!sdl) {
		SGE_LOGE("Failed to init SDL.");
		return EXIT_FAILURE;
	}

	std::string mode;
	std::unique_ptr<SGE::Handle> handler;

	cmdline({ "-m", "--mode" }, "play") >> mode;
	SGE_LOGI("Mode: %s", mode.c_str());
	SGE_LOGI("Path: %s", cmdline[1].c_str());

	if (mode == "play") {
		SGE_LOGD("Creating game player...");
		handler.reset(new SGE::Player(uv_default_loop()));
	} else if (mode == "edit") {
		SGE_LOGD("Creating game editor...");
		handler.reset(new SGE::Editor(uv_default_loop()));
	} else if (mode == "serve") {
		SGE_LOGD("Creating game server...");
		handler.reset(new SGE::Server(uv_default_loop()));
	} else {
		SGE_LOGE("Unknown mode '%s'", mode.c_str());
		return EXIT_FAILURE;
	}

	if (!handler) {
		SGE_LOGE("Failed to init SGE.");
		return EXIT_FAILURE;
	}

	if (!handler->start(cmdline[1])) {
		SGE_LOGE("Failed to start game.");
		return EXIT_FAILURE;
	}

	uv_prepare_t eventPoller;
	uv_prepare_init(uv_default_loop(), &eventPoller);
	uv_prepare_start(&eventPoller, sgePollEvents);
	eventPoller.data = handler.get();

	// Main loop.
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_prepare_stop(&eventPoller);
	
	return EXIT_SUCCESS;
}
