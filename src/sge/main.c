/*
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include <sge/kernel.h>

static void sge_poll_sdl_event(uv_timer_t* p) {
	SDL_Event evt;
	while (SDL_PollEvent(&evt)) {
		if (evt.type != SDL_QUIT) {
			sge_kernel_feed_event(&evt);
		} else {
			uv_stop(p->loop);
		}
	}
}

int main(int argc, char* argv[]) {
	int ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0) {
		goto end0;
	}

	uv_loop_t* loop = uv_default_loop();

	uv_timer_t sdl_event_timer;
	ret = uv_timer_init(loop, &sdl_event_timer);
	if (ret < 0) {
		goto end1;
	}
	ret = uv_timer_start(&sdl_event_timer, &sge_poll_sdl_event, 0, 15);
	if (ret < 0) {
		goto end2;
	}

	ret = sge_kernel_init(loop, ".", "/init.lua");
	if (ret < 0) {
		goto end3;
	}
	ret = sge_kernel_start(NULL, NULL);
	if (ret < 0) {
		goto end4;
	}

	uv_run(loop, UV_RUN_DEFAULT);

end4:
	sge_kernel_shutdown();

end3:
	uv_timer_stop(&sdl_event_timer);

end2:
	uv_close((uv_handle_t*) & sdl_event_timer, NULL);

end1:
	SDL_Quit();

end0:
	return EXIT_SUCCESS;
}
