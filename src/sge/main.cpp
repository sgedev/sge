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
		"debug version";
#else
		"release version";
#endif

	SGE_LOGI("Simple Game Engine v%d.%d.%d [%s]\n",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH, type);
}

static void show_help(const char *arg0)
{
	SGE_LOGI("Usage:\n");
	SGE_LOGI("  %s [Options] <Game path>\n", arg0);
	SGE_LOGI("Options:\n");
	SGE_LOGI("  -h, --help\n");
	SGE_LOGI("    Show this message.\n");
	SGE_LOGI("  -v, --version\n");
	SGE_LOGI("    Show version.\n");
	SGE_LOGI("  -d, --debug\n");
	SGE_LOGI("    Show debug logs.\n");
}

int main(int argc, char *argv[])
{
	int ret = EXIT_FAILURE;
	argh::parser cmdline(argv);
	std::string game;
	filesystem::path path;

	SDL_SetMainReady();

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		goto end0;

	if (cmdline[{ "-d", "--debug" }])
		SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

	if (!PHYSFS_init(NULL))
		goto end1;

	if (cmdline[{ "-v", "--version" }]) {
		show_version();
		goto end2;
	}

	show_version();

	if (cmdline[{ "-h", "--help" }]) {
		show_help(argv[0]);
		goto end2;
	}

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
		SGE_LOGE("Invalid game path '%s'.\n", game.c_str());
		goto end2;
	}

	SGE_LOGD("Platform: %s, CPUs %d, Memory %dMB\n",
		SDL_GetPlatform(), SDL_GetCPUCount(), SDL_GetSystemRAM());

	SGE_LOGI("Game path: '%s'\n", path.str().c_str());
	if (!PHYSFS_mount(path.str().c_str(), "/", 0)) {
		SGE_LOGE("Failed to mount root (%d).\n", PHYSFS_getLastErrorCode());
		goto end2;
	}

	//if (cmdline[{ "-s", "--server" }])
	//	ret = sge::server::main();
	//else
		ret = sge::game::main();

end2:
	PHYSFS_deinit();

end1:
	SDL_Quit();

end0:
	SGE_LOGD("Quit.\n");

	return ret;
}

