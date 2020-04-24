//
//
#include <stdio.h>
#include <stdlib.h>

#include <parg.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <sge/common.h>
#include <sge/window.h>
#include <sge/input.h>
#include <sge/scene.h>
#include <sge/vm.h>

static int sge_run;
static int sge_fps;
static int sge_fps_count;
static Uint32 sge_fps_last;
static Uint32 sge_elapsed_min;
static Uint32 sge_last;

static int sge_get_fps(void)
{
	return sge_fps;
}

static int sge_get_fps_max(void)
{
	CX_ASSERT(sge_elapsed_min > 0);
	return 1000 / sge_elapsed_min;
}

static void sge_set_fps_max(int v)
{
	CX_ASSERT(v > 0);
	sge_elapsed_min = 1000 / v;
}

static void sge_draw_3d(void)
{

}

static void sge_draw_2d(NVGcontext *nvg)
{
	nvgBeginPath(nvg);
	nvgRect(nvg, 100, 100, 120, 30);
	nvgFillColor(nvg, nvgRGBA(255, 192, 0, 255));
	nvgFill(nvg);
}

static void sge_poll_events(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_MOUSEMOTION:
			sge_input_handle_mouse_move_event(&event.motion);
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			sge_input_handle_key_event(&event.key);
			break;
		case SDL_WINDOWEVENT:
			sge_window_handle_event(&event.window);
			break;
		case SDL_QUIT:
			sge_run = 0;
			break;
		default:
			break;
		}
	}
}

static void sge_frame(float elapsed)
{
	static const sge_vm_traps_t traps = {
		.get_fps = sge_get_fps,
		.get_fps_max = sge_get_fps_max,
		.set_fps_max = sge_set_fps_max
	};

	static const sge_window_drawer_t drawer = {
		.draw_3d = sge_draw_3d,
		.draw_2d = sge_draw_2d
	};

	sge_input_update(elapsed);
	sge_vm_update(elapsed, &traps);
	sge_scene_update(elapsed);
	sge_window_update(elapsed, &drawer);
}

static void sge_log_output(void *data, int category, SDL_LogPriority priority, const char *message)
{
	if (priority == SDL_LOG_PRIORITY_ERROR || priority == SDL_LOG_PRIORITY_CRITICAL)
		fprintf(stderr, "SGE: %s\n", message);
	else
		fprintf(stdout, "SGE: %s\n", message);
}

static void sge_print_help(void)
{
	SGE_LOGI("Usage:");
	SGE_LOGI("  sge [options] <root>");
	SGE_LOGI("Options:");
	SGE_LOGI("  -h Show this message.");
	SGE_LOGI("  -v Show version message.");
}

static void sge_print_version(void)
{
	SGE_LOGI("%d.%d.%d",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);
}

static int sge_init(int argc, char *argv[])
{
	struct parg_state ps;
	char opt;
	const char *root = NULL;
	int ret;

	parg_init(&ps);

	while ((opt = parg_getopt(&ps, argc, argv, "hv")) != -1) {
		switch (opt) {
		case 1:
			root = ps.optarg;
			break;
		case 'h':
			sge_print_help();
			return -1;
		case 'v':
			sge_print_version();
			return -1;
		default:
			SGE_LOGE("Unknown option -%c\n", opt);
			return -1;
		}
	}

	if (root == NULL) {
		SGE_LOGE("Root is not set.");
		return EXIT_FAILURE;
	}

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0)
		goto bad0;

	ret = PHYSFS_init(argv[0]);
	if (!ret)
		goto bad1;

	SGE_LOGI("Root: %s", root);
	PHYSFS_mount(root, "/", 1);

	ret = sge_window_init();
	if (ret < 0)
		goto bad2;

	ret = sge_input_init();
	if (ret < 0)
		goto bad3;

	ret = sge_scene_init();
	if (ret < 0)
		goto bad4;

	ret = sge_vm_init();
	if (ret < 0)
		goto bad5;

	return 0;

bad5:
	sge_scene_shutdown();

bad4:
	sge_input_shutdown();

bad3:
	sge_window_shutdown();

bad2:
	PHYSFS_deinit();

bad1:
	SDL_Quit();

bad0:
	return -1;
}

static void sge_shutdown(void)
{
	sge_vm_shutdown();

	sge_scene_shutdown();

	sge_input_shutdown();

	sge_window_shutdown();

	PHYSFS_deinit();

	SDL_Quit();
}

int main(int argc, char *argv[])
{
	int ret;
	Uint32 curr;
	Uint32 pass;
	Uint32 delay;

	SDL_LogSetOutputFunction(sge_log_output, NULL);
#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	SGE_LOGI("Simple Game Engine - %d.%d.%d",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);

	ret = sge_init(argc, argv);
	if (ret < 0)
		return EXIT_FAILURE;

	sge_last = SDL_GetTicks();
	sge_fps_last = sge_last;
	sge_fps = 0;
	sge_fps_count = 0;
	sge_elapsed_min = 1000 / 60;
	sge_run = 1;

	while (sge_run) {
		sge_poll_events();

		curr = SDL_GetTicks();
		if (curr < sge_last) {
			sge_last = curr;
			continue;
		}

		pass = curr - sge_last;
		if (pass < sge_elapsed_min) {
			delay = sge_elapsed_min - pass;
			SDL_Delay(SDL_min(delay, 100));
			continue;
		}

		sge_frame(((float)pass) / 1000.0f);

		pass = curr - sge_fps_last;
		if (pass >= 1000) {
			sge_fps = sge_fps_count;
			sge_fps_count = 0;
			sge_fps_last = SDL_GetTicks();
		} else
			sge_fps_count += 1;

		sge_last = curr;
	}

	sge_shutdown();

	return 0;
}

