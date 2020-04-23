//
//
#include <stdio.h>
#include <stdlib.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <sge/common.h>
#include <sge/window.h>
#include <sge/input.h>
#include <sge/scene.h>
#include <sge/vm.h>

static int sge_run;

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

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0)
		goto end0;

	ret = sge_window_init();
	if (ret < 0)
		goto end1;

	ret = sge_input_init();
	if (ret < 0)
		goto end2;

	ret = sge_scene_init();
	if (ret < 0)
		goto end3;

	ret = sge_vm_init();
	if (ret < 0)
		goto end4;

	sge_run = 1;
	while (sge_run) {
		sge_poll_events();
		sge_input_update(16.0f);
		sge_vm_update(16.0f, &sge_traps);
		sge_scene_update(16.0f);
		sge_window_update(16.0f, &sge_drawer);
		SDL_Delay(16);
	}

	exit_code = EXIT_SUCCESS;

	sge_vm_shutdown();

end4:
	sge_scene_shutdown();

end3:
	sge_input_shutdown();

end2:
	sge_window_shutdown();

end1:
	SDL_Quit();

end0:
	return exit_code;
}

