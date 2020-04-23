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
static Uint32 sge_elapsed_min;
static Uint32 sge_last;
static const char *sge_root;

static int sge_get_fps(void)
{
	return 123;
}

static int sge_get_fps_max(void)
{
	return 234;
}

static void sge_set_fps_max(int v)
{
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

static int sge_parse_cmdline(int argc, char *argv[])
{
	struct parg_state ps;
	char c;

	parg_init(&ps);

	while ((c = parg_getopt(&ps, argc, argv, "hv")) != -1) {
		switch (c) {
		case 1:
			sge_root = ps.optarg;
			break;
		case 'h':
			printf("Usage: ...\n");
			break;
		case 'v':
			printf("%d.%d.%d\n", SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);
			break;
		case '?':
			if (ps.optopt == 'r') {
				printf("option -r requires an argument\n");
			} else {
				printf("unknown option -%c\n", ps.optopt);
			}
			return -1;
		default:
			printf("error: unhandled option -%c\n", c);
			return -1;
		}
	}

	return 0;
}

static const sge_vm_traps_t sge_traps = {
	.get_fps = sge_get_fps,
	.get_fps_max = sge_get_fps_max,
	.set_fps_max = sge_set_fps_max
};

static const sge_window_drawer_t sge_drawer = {
	.draw_3d = sge_draw_3d,
	.draw_2d = sge_draw_2d
};

int main(int argc, char *argv[])
{
	int ret;
	int exit_code = EXIT_FAILURE;
	Uint32 pass;
	Uint32 delay;
	float elapsed;

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	ret = sge_parse_cmdline(argc, argv);
	if (ret < 0)
		goto end0;

	if (sge_root == NULL)
		goto end0;

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0)
		goto end0;

	ret = PHYSFS_init(argv[0]);
	if (!ret)
		goto end1;

	PHYSFS_mount(sge_root, "/", 1);

	ret = sge_window_init();
	if (ret < 0)
		goto end2;

	ret = sge_input_init();
	if (ret < 0)
		goto end3;

	ret = sge_scene_init();
	if (ret < 0)
		goto end4;

	ret = sge_vm_init();
	if (ret < 0)
		goto end5;

	sge_last = SDL_GetTicks();
	sge_elapsed_min = 1000 / 60;
	sge_fps = 0;
	sge_run = 1;

	while (sge_run) {
		sge_poll_events();

		pass = SDL_GetTicks() - sge_last;
		if (pass < sge_elapsed_min) {
			delay = sge_elapsed_min - pass;
			SDL_Delay(SDL_min(delay, 50));
			continue;
		}

		elapsed = (float)pass;

		sge_input_update(elapsed);
		sge_vm_update(elapsed, &sge_traps);
		sge_scene_update(elapsed);
		sge_window_update(elapsed, &sge_drawer);
	}

	exit_code = EXIT_SUCCESS;

	sge_vm_shutdown();

end5:
	sge_scene_shutdown();

end4:
	sge_input_shutdown();

end3:
	sge_window_shutdown();

end2:
	PHYSFS_deinit();

end1:
	SDL_Quit();

end0:
	return exit_code;
}

