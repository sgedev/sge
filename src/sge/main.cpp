//
//
#include <string>
#include <iostream>

#include <argh.h>
#include <physfs.h>
#include <filesystem/path.h>
#include <filesystem/resolver.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <sge/common.hpp>
#include <sge/game.hpp>

static void show_version(void)
{
	const char *type =
#ifdef SGE_DEBUG
		"DEBUG";
#else
		"RELEASE";
#endif

	SGE_LOGI("SGE: %d.%d.%d [%s]\n",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH, type);
}

static void show_help(const char *arg0)
{
	SGE_LOGI("%s [options] <game path>\n", arg0);
	SGE_LOGI("options:\n");
	SGE_LOGI("-h, --help\n");
	SGE_LOGI("    show this message.\n");
	SGE_LOGI("-v, --version\n");
	SGE_LOGI("    show version.\n");
}

int main(int argc, char *argv[])
{
	int ret = EXIT_FAILURE;
	argh::parser cmdline(argv);
	std::string game;
	filesystem::path path;
	Uint32 last;
	Uint32 pass;

	SDL_SetMainReady();

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		goto end0;

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	if (!PHYSFS_init(NULL))
		goto end1;

	if (cmdline[{ "-v", "--version" }]) {
		show_version();
		goto end2;
	}

	if (cmdline[{ "-h", "--help" }]) {
		show_help(argv[0]);
		goto end2;
	}

	show_version();

	SGE_LOGD("Platform: %s, CPUs %d, Memory %dMB\n",
		SDL_GetPlatform(), SDL_GetCPUCount(), SDL_GetSystemRAM());

	if (cmdline.size() < 2) {
		SGE_LOGE("No game path.\n");
		goto end2;
	}

	if (cmdline.size() > 2) {
		SGE_LOGE("Too many game path.\n");
		goto end0;
	}

	cmdline(1) >> game;
	path = game;
	path = path.make_absolute();
	if (!path.exists()) {
		SGE_LOGE("Invalid game path.\n");
		goto end2;
	}

	SGE_LOGI("Game path: '%s'\n", path.str().c_str());
	if (!PHYSFS_mount(path.str().c_str(), "/", 0)) {
		SGE_LOGE("failed to mount root (%d).\n", PHYSFS_getLastErrorCode());
		goto end2;
	}

	sge::game::init();

	last = SDL_GetTicks();

	while (sge::game::is_running()) {
		pass = SDL_GetTicks() - last;
		if (pass < 0) {
			last = SDL_GetTicks();
			continue;
		}

		SDL_Event event;

		while (SDL_PollEvent(&event))
			sge::game::handle_event(event);

		if (pass < 16) {
			SDL_Delay(16 - pass);
			continue;
		}

		sge::game::frame(float(pass) / 1000.0f);
		last = SDL_GetTicks();
	}

	sge::game::shutdown();

	ret = EXIT_SUCCESS;
	SGE_LOGI("Shutdown.\n");

end2:
	PHYSFS_deinit();

end1:
	SDL_Quit();

end0:
	SGE_LOGD("Quit.\n");

	return ret;
}

